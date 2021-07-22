#ifndef RPCPP_CPP_LINUXTCPSOCKETCLIENT_H_
#define RPCPP_CPP_LINUXTCPSOCKETCLIENT_H_

#include "rpcpp/client/iclientconnector.h"
#include "rpcpp/common/streamreader.h"
#include "rpcpp/common/streamwriter.h"
#include "rpcpp/common/message.h"
namespace rpcpp
{
    class LinuxTcpSocketClient : public IClientConnector
    {
        public:
            LinuxTcpSocketClient(const std::string& hostToConnect, const unsigned int &port);
            virtual ~LinuxTcpSocketClient();
            virtual void SendRPCMessage(const std::string& message, std::string& result) ;
            virtual void SendRPCMessage(const std::string &message);
        private:
            std::string hostToConnect;
            unsigned int port;
            Message msg;
            int Connect() ;
            int Connect(const std::string& ip, const int& port) ;
            bool IsIpv4Address(const std::string& ip);
    };

}
#endif /* RPCPP_CPP_LINUXTCPSOCKETCLIENT_H_ */
