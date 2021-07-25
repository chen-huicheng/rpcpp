
#include "rpcpp/server/connectors/LinuxTcpSocketServer.h"
#include "rpcpp/common/Poller.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <map>
#include <errno.h>
#include <iostream>
#include <sstream>
#include <string>

using namespace rpcpp;

LinuxTcpSocketServer::LinuxTcpSocketServer(const std::string &ip, const unsigned int &port) : ip(ip), port(port), stop(false) {}

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

    // fcntl(listenfd, F_SETFL, FNDELAY);
    // int reuseaddr = 1;
    // setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr,
    //            sizeof(reuseaddr));

    memset(&(address), 0, sizeof(address));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    // inet_aton(ip.c_str(), &(address.sin_addr));
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
    while (!stop)
    {
        struct sockaddr_in client_address;
        socklen_t client_addrlength = sizeof(client_address);
        int connfd = accept(listenfd, (struct sockaddr *)&client_address, &client_addrlength);
        if (connfd < 0)
        {
            std::cout << strerror(errno) << std::endl;
            break;
        }
        std::string target, result;
        if (msg.ReadMessage(connfd, target) != 0)
        {
            continue;
        }
        ProcessRequest(target, result);
        msg.SendMessage(connfd, result);
    }
    // Poller poller;
    // poller.addfd(listenfd, false);
    // // std::map<int, std::string> request;
    // std::vector<epoll_event> events;
    // while (!stop)
    // {
    //     int number = poller.wait(events, -1);
    //     if (number < 0 && errno != EINTR)
    //     {
    //         break;
    //     }
    //     for (int i = 0; i < number; i++)
    //     {
    //         int sockfd = events[i].data.fd;
    //         std::cout<<sockfd<<std::endl;
    //         if (sockfd == listenfd)
    //         {
    //             struct sockaddr_in client_address;
    //             socklen_t client_addrlength = sizeof(client_address);
    //             int connfd = accept(listenfd, (struct sockaddr *)&client_address, &client_addrlength);
    //             if (connfd < 0)
    //             {
    //                 std::cout<<strerror(errno)<<std::endl;
    //                 break;
    //             }
    //             int rt = poller.addfd(connfd, true);
    //             if (rt < 0)
    //             {
    //                 break;
    //             }
    //         }
    //         else if (events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
    //         {
    //             //如有异常 关闭客户端链接
    //             poller.removefd(sockfd);
    //         }
    //         else if (events[i].events & EPOLLIN)
    //         {
    //             std::string target, result;
    //             if (msg.ReadMessage(sockfd, target) != 0)
    //             {
    //                 poller.removefd(sockfd);
    //                 continue;
    //             }
    //             ProcessRequest(target, result);
    //             msg.SendMessage(sockfd, result);
    //         }
    //         else
    //         {
    //             std::cout<<"removefd"<<sockfd<<std::endl;
    //             poller.removefd(sockfd);
    //         }
    //     }
    // }
}
void *LinuxTcpSocketServer::eventloop(void *arg)
{
    LinuxTcpSocketServer *server = (LinuxTcpSocketServer *)arg;
    server->run();
    return server;
}
bool LinuxTcpSocketServer::StartListening()
{
    if(!InitializeListener())
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
