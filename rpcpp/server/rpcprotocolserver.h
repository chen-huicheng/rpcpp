
#ifndef RPCPP_CPP_RPCPROTOCOLSERVERV2_H_
#define RPCPP_CPP_RPCPROTOCOLSERVERV2_H_

#include <string>
#include <vector>
#include <map>
#include <jsoncpp/json/json.h>
#include "rpcpp/common/exception.h"

#define KEY_REQUEST_VERSION "jsonrpc"

namespace rpcpp
{
    class RpcProtocolServer
    {
    public:
        std::string HandleRequest(const std::string &method, const Json::Value &parameter, bool isNotification);

        void HandleRequest(const std::string &method, const Json::Value &parameter, std::string &result, bool isNotification);

        void BuildResponse(Json::Value &result, const std::string &response);

        std::string BuildResponse(const Json::Value &response, Json::Value &result);

        RpcProtocolServer();
    private:
        void HandleJsonRequest(const Json::Value &request, Json::Value &response);
        bool ValidateRequestFields(const Json::Value &val);
        void WrapResult(const Json::Value &request, Json::Value &response, Json::Value &retValue);
        void WrapError(const Json::Value &request, int code, const std::string &message, Json::Value &result);
        void WrapException(const Json::Value &request, const JsonRpcException &exception, Json::Value &result);

    };

} /* namespace rpcpp */
#endif /* RPCPP_CPP_RPCPROTOCOLSERVERV2_H_ */
