#ifndef RPCPP_CPP_TCPSOCKETCLIENT_H_
#define RPCPP_CPP_TCPSOCKETCLIENT_H_

#include "rpcpp/client/iclientconnector.h"
#include "rpcpp/common/rpcexception.h"
#include <string>

namespace rpcpp
{
    class TcpSocketClient : public IClientConnector
    {
        public:
            TcpSocketClient(const std::string& ipToConnect, const unsigned int &port);
            virtual ~TcpSocketClient();
            virtual void SendRPCMessage(const std::string& message, std::string& result) ;
            virtual void SendRPCMessage(const std::string &message);
        private:
            IClientConnector *realSocket; 
    };

} /* namespace rpcpp */
#endif /* RPCPP_CPP_TCPSOCKETCLIENT_H_ */
