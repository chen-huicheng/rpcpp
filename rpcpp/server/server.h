
#ifndef RPCPP_CPP_ABSTRACTSERVER_H_
#define RPCPP_CPP_ABSTRACTSERVER_H_

#include <map>
#include <string>
#include <vector>
#include <functional>
#include <jsoncpp/json/json.h>
#include "rpcpp/server/abstractserverconnector.h"
#include "rpcpp/server/rpcprotocolserver.h"

namespace rpcpp
{
    class RpcServer
    {
        public:
            typedef std::function<void(const Json::Value &parameter, Json::Value &result)> methodPointer_t;
            typedef std::function<void(const Json::Value &parameter)> notificationPointer_t;

            RpcServer(AbstractServerConnector &connector):connection(connector){}

            virtual ~RpcServer();
            bool StartListening(){
                connection.StartListening();
            }

            bool StopListening(){
                connection.StopListening();
            }
            void HandleMethodCall(std::string &methodname, const Json::Value& input, Json::Value& output)
            {
                methods[methodname](input, output);
            }

            void HandleNotificationCall(std::string &notificationname, const Json::Value& input)
            {
                notifications[notificationname](input);
            }
            bool AddMethod(std::string &methodname, methodPointer_t pointer)
            {
                methods[methodname] = pointer;
            }

            bool AddNotification(std::string &notificationname, notificationPointer_t pointer)
            {
                if()
                notifications[notificationname] = pointer;
                return false;
            }

        private:
            AbstractServerConnector                         &connection;
            RpcProtocolServer                               &rpcprotocol;
            std::map<std::string, methodPointer_t>          methods;
            std::map<std::string, notificationPointer_t>    notifications;

            bool symbolExists(const std::string &name)
            {
                if (methods.find(name) != methods.end())
                    return true;
                if (notifications.find(name) != notifications.end())
                    return true;
                return false;
            }
    };

} /* namespace rpcpp */
#endif /* RPCPP_CPP_ABSTRACTSERVER_H_ */
