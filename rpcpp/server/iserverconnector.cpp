
#include "rpcpp/server/iserverconnector.h"
#include <cstdlib>

using namespace rpcpp;

IServerConnector::IServerConnector() { handler = nullptr; }

IServerConnector::~IServerConnector() {}

void IServerConnector::ProcessRequest(const std::string &request, std::string &response)
{
    if (handler != nullptr)
    {
        handler(request, response);
    }
}

void IServerConnector::SetHandler(const std::function<void(const std::string &request, std::string &response)> &_handler)
{
    handler = _handler;
}
