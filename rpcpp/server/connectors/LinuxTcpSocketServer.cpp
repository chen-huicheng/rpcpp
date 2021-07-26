
#include "rpcpp/server/connectors/LinuxTcpSocketServer.h"
#include "rpcpp/common/Poller.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <unordered_map>
#include <errno.h>
#include <iostream>
#include <sstream>
#include <string>

using namespace rpcpp;

LinuxTcpSocketServer::LinuxTcpSocketServer(const unsigned int &port, const std::string &ip) : ip(ip), port(port), stop(false) {}

LinuxTcpSocketServer::~LinuxTcpSocketServer()
{
    shutdown(listenfd, 2);
    close(listenfd);
}

bool LinuxTcpSocketServer::InitializeListener()
{
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0)
    {
        return false;
    }

    fcntl(listenfd, F_SETFL, FNDELAY);
    int reuseaddr = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr,
               sizeof(reuseaddr));

    memset(&(address), 0, sizeof(address));

    address.sin_family = AF_INET;
    if(IsIpv4Address(ip)){
        inet_aton(ip.c_str(), &(address.sin_addr));
    }
    else address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (::bind(listenfd, (struct sockaddr *)(&(address)), sizeof(address)) != 0)
    {
        return false;
    }

    if (listen(listenfd, 1024) != 0)
    {
        return false;
    }
    return true;
}

void LinuxTcpSocketServer::run()
{
    Poller poller;
    poller.addfd(listenfd, false);
    std::unordered_map<int, Message> request;
    std::vector<epoll_event> events;
    while (!stop)
    {
        events.clear();
        int number = poller.wait(events, -1);
        if (number < 0 && errno != EINTR)
        {
            break;
        }
        for (int i = 0; i < number; i++)
        {
            int sockfd = events[i].data.fd;
            if (sockfd == listenfd)
            {
                struct sockaddr_in client_address;
                socklen_t client_addrlength = sizeof(client_address);
                int connfd = accept(listenfd, (struct sockaddr *)&client_address, &client_addrlength);
                if (connfd < 0)
                {
                    std::cout << strerror(errno) << std::endl;
                    continue;
                }
                int rt = poller.addfd(connfd, true);
                request[connfd].init(connfd);
                if (rt < 0)
                {
                    poller.removefd(connfd);
                    continue;
                    ;
                }
            }
            else if (events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
            {
                poller.removefd(sockfd);
            }
            else if (events[i].events & EPOLLIN)
            {
                if (request[sockfd].ReadMessage() != 0)
                {
                    if (errno == EAGAIN || errno == EWOULDBLOCK)
                        continue;
                    poller.removefd(sockfd);
                    continue;
                }
                std::string target, result;
                target = request[sockfd].getMsg();
                ProcessRequest(target, result);
                request[sockfd].setMsg(result);
                poller.modfd(sockfd, EPOLLOUT);
            }
            else if (events[i].events & EPOLLOUT)
            {
                if (request[sockfd].SendMessage() != 0)
                {
                    if (errno == EAGAIN || errno == EWOULDBLOCK)
                        continue;
                    poller.removefd(sockfd);
                    continue;
                }
                // poller.removefd(sockfd);
            }
            else
            {
                poller.removefd(sockfd);
            }
        }
    }
}
void *LinuxTcpSocketServer::eventloop(void *arg)
{
    LinuxTcpSocketServer *server = (LinuxTcpSocketServer *)arg;
    server->run();
    return server;
}
bool LinuxTcpSocketServer::StartListening()
{
    if (!InitializeListener())
        return false;
    run();
    pthread_t pid;
    stop = false;
    int err = pthread_create(&pid, nullptr, eventloop, this);
    if (err != 0)
    {
        return false;
        stop = true;
    }

    return true;
}
bool LinuxTcpSocketServer::StopListening()
{
    stop = true;
    return true;
}

bool LinuxTcpSocketServer::IsIpv4Address(const std::string &ip)
{
    struct in_addr addr;
    return (inet_aton(ip.c_str(), &addr) != 0);
}