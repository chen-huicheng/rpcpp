
#include "rpcpp/client/client.h"
#include "rpcpp/client/rpcprotocolclient.h"

using namespace rpcpp;

Client::Client(IClientConnector &connector) : connector(connector)
{
    this->protocol = new RpcProtocolClient();
}

Client::~Client() { delete this->protocol; }

void Client::CallMethod(const std::string &name, const Json::Value &parameter, Json::Value &result)
{
    std::string request, response;
    protocol->BuildRequest(name, parameter, request, false);
    connector.SendRPCMessage(request, response);
    protocol->HandleResponse(response, result);
}

Json::Value Client::CallMethod(const std::string &name, const Json::Value &parameter)
{
    Json::Value result;
    this->CallMethod(name, parameter, result);
    return result;
}

void Client::CallNotification(const std::string &name, const Json::Value &parameter)
{
    std::string request, response;
    protocol->BuildRequest(name, parameter, request, true);
    connector.SendRPCMessage(request, response);
}
