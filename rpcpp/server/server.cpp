
#include "rpcpp/server/server.h"
#include "rpcpp/common/errors.h"
using namespace rpcpp;

RpcServer::RpcServer(IServerConnector &connector) : connection(connector)
{
    auto cb = std::bind(handler, this, _1, _2);
    connection.SetHandler(cb);
}

RpcServer::~RpcServer() {}

void RpcServer::handler(const std::string &request, std::string &response)
{
    Json::Value input,output,params;
    rpcprotocol.HandleRequest(request,input);
    std::string method = input["method"].asString();
    if(input.isMember("params"))
        params=input["params"];
    if(input.isMember("id")){    
        HandleMethodCall(method,params,output);
        rpcprotocol.BuildResponse()
    }
    else{
        
    }

}

bool RpcServer::StartListening()
{
    connection.StartListening();
}

bool RpcServer::StopListening()
{
    connection.StopListening();
}

void RpcServer::HandleMethodCall(std::string &methodname, const Json::Value &input, Json::Value &output)
{
    if (methods.find(methodname) != methods.end())
        methods[methodname](input, output);
    else
    {
        rpcprotocol.WrapError(input, Errors::ERROR_RPC_METHOD_NOT_FOUND, Errors::GetErrorMessage(Errors::ERROR_RPC_METHOD_NOT_FOUND), output);
    }
}

void RpcServer::HandleNotificationCall(std::string &notificationname, const Json::Value &input)
{
    if (notifications.find(notificationname) != notifications.end())
        notifications[notificationname](input);
}

bool RpcServer::AddMethod(std::string &methodname, methodPointer_t pointer)
{
    methods[methodname] = pointer;
}

bool RpcServer::AddNotification(std::string &notificationname, notificationPointer_t pointer)
{
    notifications[notificationname] = pointer;
    return false;
}
