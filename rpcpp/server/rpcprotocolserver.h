
#ifndef RPCPP_CPP_RPCPROTOCOLSERVERV2_H_
#define RPCPP_CPP_RPCPROTOCOLSERVERV2_H_

#include <string>
#include <vector>
#include <map>
#include <jsoncpp/json/json.h>
#include "rpcpp/common/rpcexception.h"

namespace rpcpp
{
    class RpcProtocolServer
    {
    public:
        static const std::string KEY_REQUEST_METHODNAME;
        static const std::string KEY_REQUEST_ID;
        static const std::string KEY_REQUEST_PARAMETERS;
        static const std::string KEY_RESPONSE_RESULT;
        static const std::string KEY_ERROR;
        static const std::string KEY_ERROR_CODE;
        static const std::string KEY_ERROR_MESSAGE;
        static const std::string KEY_ERROR_DATA;
    public:
        Json::Value HandleRequest(const std::string &request);

        void HandleRequest(const std::string &request, Json::Value &result);

        void BuildResponse(const Json::Value &request, const Json::Value &response, std::string &result);

        std::string BuildResponse(const Json::Value &request, const Json::Value &response);

        void WrapError(const Json::Value &request, int code, const std::string &message, Json::Value &result);
    private:
        bool ValidateRequest(const Json::Value &val);
        void WrapResult(const Json::Value &request, Json::Value &response, Json::Value &retValue);
        void WrapException(const Json::Value &request, const RpcException &exception, Json::Value &result);

    };

} /* namespace rpcpp */
#endif /* RPCPP_CPP_RPCPROTOCOLSERVERV2_H_ */
