
#ifndef RPCPP_CLIENT_CLIENT_H_
#define RPCPP_CLIENT_CLIENT_H_

#include <jsoncpp/json/json.h>
#include <vector>
#include <memory>
#include <map>

#include "rpcpp/client/iclientconnector.h"
#include "rpcpp/common/noncopyable.h"
#include "rpcpp/client/rpcprotocolclient.h"

namespace rpcpp
{
    class Client : public noncopyable
    {
    public:
        Client(IClientConnector &connector);
        virtual ~Client();

        void CallMethod(const std::string &name, const Json::Value &parameter, Json::Value &result);
        Json::Value CallMethod(const std::string &name, const Json::Value &parameter);

        void CallNotification(const std::string &name, const Json::Value &parameter);

    private:
        IClientConnector &connector;
        RpcProtocolClient protocol;
    };

}
#endif /* RPCPP_CLIENT_CLIENT_H_ */
