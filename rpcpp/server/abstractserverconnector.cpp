
#include "abstractserverconnector.h"
#include <cstdlib>

using namespace std;
using namespace rpcpp;

AbstractServerConnector::AbstractServerConnector() { this->handler = nullptr; }

AbstractServerConnector::~AbstractServerConnector() {}

void AbstractServerConnector::ProcessRequest(const string &request,string &response)
{
    if (this->handler != nullptr)
    {
        this->handler(request, response);
    }
}

void AbstractServerConnector::SetHandler(const std::function<void(const std::string &request, std::string &response)> &handler)
{
    this->handler = handler;
}
