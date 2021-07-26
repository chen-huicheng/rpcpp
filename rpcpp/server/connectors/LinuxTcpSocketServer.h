

#ifndef RPCPP_CPP_LINUXTCPSOCKETSERVERCONNECTOR_H_
#define RPCPP_CPP_LINUXTCPSOCKETSERVERCONNECTOR_H_

#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include "rpcpp/server/IServerConnector.h"
#include "rpcpp/common/StreamReader.h"
#include "rpcpp/common/StreamWriter.h"
#include "rpcpp/common/Message.h"

namespace rpcpp
{

    class LinuxTcpSocketServer : public IServerConnector
    {
    public:
        LinuxTcpSocketServer(const unsigned int &port,const std::string &ip="");
        virtual ~LinuxTcpSocketServer();
        virtual bool StartListening();
        virtual bool StopListening();
    private:
        std::string ip;
        unsigned int port;
        int listenfd;
        bool stop;
        struct sockaddr_in address;
        Message msg;
        bool InitializeListener();
        void run();
        static void* eventloop(void *arg);
        bool IsIpv4Address(const std::string &ip);
    };

}
#endif /* RPCPP_CPP_LINUXTCPSOCKETSERVERCONNECTOR_H_ */
