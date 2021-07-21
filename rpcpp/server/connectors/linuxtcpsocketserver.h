

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
#include "rpcpp/server/abstractserverconnector.h"
#include "rpcpp/common/streamreader.h"
#include "rpcpp/common/streamwriter.h"


namespace rpcpp
{

    class LinuxTcpSocketServer : public AbstractServerConnector
    {
    public:
        LinuxTcpSocketServer(const std::string &ip, const unsigned int &port);
        virtual ~LinuxTcpSocketServer();
        virtual bool StartListening();
        virtual bool StopListening();
    private:
        std::string ip;
        unsigned int port;
        int listenfd;
        bool stop;
        struct sockaddr_in address;
        StreamReader reader;
        StreamWriter writer;
        bool InitializeListener();
        void run();
        static void* eventloop(void *arg);
    };

}
#endif /* RPCPP_CPP_LINUXTCPSOCKETSERVERCONNECTOR_H_ */
