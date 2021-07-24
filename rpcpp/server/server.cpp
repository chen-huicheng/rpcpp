
#include "rpcpp/server/server.h"
#include "rpcpp/common/errors.h"
using namespace rpcpp;

RpcServer::RpcServer(IServerConnector &connector) : connection(connector)
{
}

RpcServer::~RpcServer() {}

void RpcServer::HandleCall(const std::string &request, std::string &response)
{
    Json::Value input, output, params;
    rpcprotocol.HandleRequest(request, input);
    std::string method = input["method"].asString();
    if (input.isMember("params"))
        params = input["params"];
    if (input.isMember("id"))
    {
        HandleMethodCall(method, params, output);
        rpcprotocol.BuildResponse(input, output, response);
    }
    else
    {
        HandleNotificationCall(method, params);
    }
}

bool RpcServer::StartListening()
{
    auto cb = std::bind(&RpcServer::HandleCall, this,_1, _2);
    connection.SetHandler(cb);
    return connection.StartListening();
}

bool RpcServer::StopListening()
{
    return connection.StopListening();
}

void RpcServer::HandleMethodCall(std::string &methodname, const Json::Value &params, Json::Value &result)
{
    if (methods.find(methodname) != methods.end())
        methods[methodname](params, result);
    else
    {
        rpcprotocol.WrapError(params, Errors::ERROR_RPC_METHOD_NOT_FOUND, Errors::GetErrorMessage(Errors::ERROR_RPC_METHOD_NOT_FOUND), result);
    }
}

void RpcServer::HandleNotificationCall(std::string &notificationname, const Json::Value &params)
{
    if (notifications.find(notificationname) != notifications.end())
        notifications[notificationname](params);
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