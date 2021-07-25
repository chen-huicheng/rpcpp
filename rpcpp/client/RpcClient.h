
#ifndef RPCPP_CLIENT_CLIENT_H_
#define RPCPP_CLIENT_CLIENT_H_

#include <jsoncpp/json/json.h>
#include <vector>
#include <memory>
#include <map>

#include "rpcpp/common/Errors.h"
#include "rpcpp/common/RpcException.h"
#include "rpcpp/client/IClientConnector.h"
#include "rpcpp/common/noncopyable.h"
#include "rpcpp/client/RpcProtocolClient.h"
#include "rpcpp/client/connectors/LinuxTcpSocketClient.h"

namespace rpcpp
{
    class RpcClient : public noncopyable
    {
    public:
        RpcClient(IClientConnector &connector);
        virtual ~RpcClient();

        void CallMethod(const std::string &name, const Json::Value &parameter, Json::Value &result);
        Json::Value CallMethod(const std::string &name, const Json::Value &parameter);

        void CallNotification(const std::string &name, const Json::Value &parameter);

    private:
        IClientConnector &connector;
        RpcProtocolClient protocol;
    };

}
#endif /* RPCPP_CLIENT_CLIENT_H_ */
