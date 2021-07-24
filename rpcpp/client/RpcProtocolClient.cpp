#include "rpcpp/client/RpcProtocolClient.h"

using namespace rpcpp;

const std::string RpcProtocolClient::KEY_PROCEDURE_NAME = "method";
const std::string RpcProtocolClient::KEY_ID = "id";
const std::string RpcProtocolClient::KEY_PARAMETER = "params";
const std::string RpcProtocolClient::KEY_RESULT = "result";
const std::string RpcProtocolClient::KEY_ERROR = "error";
const std::string RpcProtocolClient::KEY_ERROR_CODE = "code";
const std::string RpcProtocolClient::KEY_ERROR_MESSAGE = "message";
const std::string RpcProtocolClient::KEY_ERROR_DATA = "data";

//构建json
void RpcProtocolClient::BuildRequest(const std::string &method, const Json::Value &parameter, std::string &result, bool isNotification)
{
    Json::Value request;
    Json::StreamWriterBuilder wbuilder;
    BuildRequest(id++, method, parameter, request, isNotification);
    result = Json::writeString(wbuilder, request);
}
std::string RpcProtocolClient::BuildRequest(const std::string &method, const Json::Value &parameter, bool isNotification)
{
    std::string result;
    BuildRequest(method, parameter, result, isNotification);
    return result;
}
//解析string to json
void RpcProtocolClient::HandleResponse(const std::string &response, Json::Value &result)
{
    Json::Reader reader;
    Json::Value value;
    try
    {
        if (reader.parse(response, value))
        {
            if (ValidateResponse(value))
            {
                result = value[KEY_RESULT];
            }
            else
            {
                throw RpcException(Errors::ERROR_CLIENT_INVALID_RESPONSE, " " + response);
            }
        }
        else
        {
            throw RpcException(Errors::ERROR_RPC_JSON_PARSE_ERROR, " " + response);
        }
    }
    catch (Json::Exception &e)
    {
        throw RpcException(Errors::ERROR_RPC_JSON_PARSE_ERROR, " " + response);
    }
}
//处理响应 取出result
Json::Value RpcProtocolClient::HandleResponse(const std::string &response)
{
    Json::Value result;
    HandleResponse(response, result);
    return result;
}
// 由函数和参数构建Json
void RpcProtocolClient::BuildRequest(int id, const std::string &method, const Json::Value &parameter, Json::Value &result, bool isNotification)
{
    result[KEY_PROCEDURE_NAME] = method;
    if (parameter != Json::nullValue)
        result[KEY_PARAMETER] = parameter;
    if (!isNotification)
        result[KEY_ID] = id;
}

//判断json是否合法
bool RpcProtocolClient::ValidateResponse(const Json::Value &response)
{
    if (!response.isObject() || !response.isMember(KEY_ID))
        return false;
    if (!response.isMember(KEY_RESULT) || response.isMember(KEY_ERROR))
        return false;
    return true;
}
