
#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "rpcpp/client/connectors/LinuxTcpSocketClient.h"

using namespace rpcpp;

LinuxTcpSocketClient::LinuxTcpSocketClient(const std::string &hostToConnect, const unsigned int &port) : hostToConnect(hostToConnect), port(port) {}

LinuxTcpSocketClient::~LinuxTcpSocketClient() {}

void LinuxTcpSocketClient::SendRPCMessage(const std::string &message, std::string &result)
{
    int socket_fd = Connect();
    msg.SendMessage(socket_fd, message);
    msg.ReadMessage(socket_fd, result);
    close(socket_fd);
}
void LinuxTcpSocketClient::SendRPCMessage(const std::string &message)
{
    int socket_fd = Connect();
    msg.SendMessage(socket_fd, message);
    close(socket_fd);
}
int LinuxTcpSocketClient::Connect()
{
    if (IsIpv4Address(hostToConnect))
    {
        return Connect(hostToConnect, port);
    }
    else
    {
        return -1;
    }
}

int LinuxTcpSocketClient::Connect(const std::string &ip, const int &port)
{
    sockaddr_in address;
    int socket_fd;
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        std::string message = "socket() failed";
        int err = errno;
        message = strerror(err);
        throw RpcException(Errors::ERROR_CLIENT_CONNECTOR, message);
    }
    memset(&address, 0, sizeof(sockaddr_in));

    address.sin_family = AF_INET;
    inet_aton(ip.c_str(), &(address.sin_addr));
    address.sin_port = htons(port);

    if (connect(socket_fd, (struct sockaddr *)&address, sizeof(sockaddr_in)) != 0)
    {
        std::string message = "connect() failed";
        int err = errno;
        message = strerror(err);
        close(socket_fd);
        throw RpcException(Errors::ERROR_CLIENT_CONNECTOR, message);
    }
    return socket_fd;
}

bool LinuxTcpSocketClient::IsIpv4Address(const std::string &ip)
{
    struct in_addr addr;
    return (inet_aton(ip.c_str(), &addr) != 0);
}