
#include "rpcpp/client/RpcClient.h"
#include "rpcpp/client/RpcProtocolClient.h"

using namespace rpcpp;

RpcClient::RpcClient(IClientConnector &connector) : connector(connector)
{
}

RpcClient::~RpcClient() { }

void RpcClient::CallMethod(const std::string &name, const Json::Value &parameter, Json::Value &result)
{
    std::string request, response;
    std::cout<<name<<std::endl;
    protocol.BuildRequest(name, parameter, request, false);
    connector.SendRPCMessage(request, response);
    protocol.HandleResponse(response, result);
}

Json::Value RpcClient::CallMethod(const std::string &name, const Json::Value &parameter)
{
    Json::Value result;
    CallMethod(name, parameter, result);
    return result;
}

void RpcClient::CallNotification(const std::string &name, const Json::Value &parameter)
{
    std::string request, response;
    protocol.BuildRequest(name, parameter, request, true);
    connector.SendRPCMessage(request, response);
}
