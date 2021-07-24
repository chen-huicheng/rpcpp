
#ifndef RPCPP_CPP_ABSTRACTSERVER_H_
#define RPCPP_CPP_ABSTRACTSERVER_H_

#include <map>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <jsoncpp/json/json.h>

// #include "rpcpp/server/irpchandler.h"
#include "rpcpp/server/iserverconnector.h"
#include "rpcpp/server/rpcprotocolserver.h"
using namespace std::placeholders; 
namespace rpcpp
{
    class RpcServer
    {
    public:
        typedef std::function<void(const Json::Value &parameter, Json::Value &result)> methodPointer_t;
        typedef std::function<void(const Json::Value &parameter)> notificationPointer_t;

        RpcServer(IServerConnector &connector) ;

        virtual ~RpcServer();
        
        bool StartListening();

        bool StopListening();

        void HandleCall(const std::string &request,std::string &response);

        void HandleMethodCall(std::string &methodname, const Json::Value &input, Json::Value &output);

        void HandleNotificationCall(std::string &notificationname, const Json::Value &input);

        bool AddMethod(std::string &methodname, methodPointer_t pointer);

        bool AddNotification(std::string &notificationname, notificationPointer_t pointer);
    private:
        IServerConnector &connection;
        RpcProtocolServer rpcprotocol;
        // std::shared_ptr<IRPCHandler> handler;
        std::map<std::string, methodPointer_t> methods;
        std::map<std::string, notificationPointer_t> notifications;
    };

}
#endif /* RPCPP_CPP_ABSTRACTSERVER_H_ */
