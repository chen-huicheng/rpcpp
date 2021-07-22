#ifndef RPCPP_CPP_CLIENTCONNECTOR_H_
#define RPCPP_CPP_CLIENTCONNECTOR_H_

#include <string>
#include "rpcpp/common/rpcexception.h"

namespace rpcpp
{
    class IClientConnector
    {
        public:
            virtual ~IClientConnector(){}

            virtual void SendRPCMessage(const std::string &message, std::string& result)  = 0;
            virtual void SendRPCMessage(const std::string &message) = 0;
    };
} /* namespace rpcpp */
#endif /* RPCPP_CPP_CLIENTCONNECTOR_H_ */
