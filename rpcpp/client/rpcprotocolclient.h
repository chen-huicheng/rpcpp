#ifndef RESPONSEHANDLER_H
#define RESPONSEHANDLER_H
#include <iostream>
#include <string>
#include <jsoncpp/json/json.h>
#include "rpcpp/common/exception.h"
#include "rpcpp/common/errors.h"

namespace rpcpp
{
    class RpcProtocolClient
    {
    public:
        RpcProtocolClient();
        std::string BuildRequest(const std::string &method, const Json::Value &parameter, bool isNotification);

        void BuildRequest(const std::string &method, const Json::Value &parameter, std::string &result, bool isNotification);

        void HandleResponse(const std::string &response, Json::Value &result);

        Json::Value HandleResponse(const Json::Value &response, Json::Value &result);

        static const std::string KEY_PROCEDURE_NAME;
        static const std::string KEY_ID;
        static const std::string KEY_PARAMETER;
        static const std::string KEY_RESULT;
        static const std::string KEY_ERROR;
        static const std::string KEY_ERROR_CODE;
        static const std::string KEY_ERROR_MESSAGE;
        static const std::string KEY_ERROR_DATA;

    private:
        int64_t id;
        void BuildRequest(int id, const std::string &method, const Json::Value &parameter, Json::Value &result, bool isNotification);
        bool ValidateResponse(const Json::Value &response);
        bool HasError(const Json::Value &response);
        void throwErrorException(const Json::Value &response);
    };
}
#endif // RESPONSEHANDLER_H
