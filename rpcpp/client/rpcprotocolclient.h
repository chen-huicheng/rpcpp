#ifndef RESPONSEHANDLER_H
#define RESPONSEHANDLER_H
#include <jsoncpp/json/json.h>
#include "rpcpp/common/rpcexception.h"
#include "rpcpp/common/errors.h"
#include <iostream>
#include <string>

namespace rpcpp
{
    class RpcProtocolClient
    {
    public:
        std::string BuildRequest(const std::string &method, const Json::Value &parameter, bool isNotification);

        void BuildRequest(const std::string &method, const Json::Value &parameter, std::string &result, bool isNotification);

        void HandleResponse(const std::string &response, Json::Value &result);

        Json::Value HandleResponse(const std::string &response);

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
    };
}
#endif // RESPONSEHANDLER_H
