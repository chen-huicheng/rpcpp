#include "rpcpp/client/rpcprotocolclient.h"

using namespace rpcpp;

const std::string RpcProtocolClient::KEY_PROCEDURE_NAME = "method";
const std::string RpcProtocolClient::KEY_ID = "id";
const std::string RpcProtocolClient::KEY_PARAMETER = "params";
const std::string RpcProtocolClient::KEY_RESULT = "result";
const std::string RpcProtocolClient::KEY_ERROR = "error";
const std::string RpcProtocolClient::KEY_ERROR_CODE = "code";
const std::string RpcProtocolClient::KEY_ERROR_MESSAGE = "message";
const std::string RpcProtocolClient::KEY_ERROR_DATA = "data";
#define KEY_REQUEST_METHODNAME "method"
#define KEY_REQUEST_ID "id"
#define KEY_REQUEST_PARAMETERS "params"
#define KEY_RESPONSE_ERROR "error"
#define KEY_RESPONSE_RESULT "result"

RpcProtocolClient::RpcProtocolClient()
{
}

void RpcProtocolClient::BuildRequest(const std::string &method,
                                     const Json::Value &parameter,
                                     std::string &result, bool isNotification)
{ //构建json
    Json::Value request;
    Json::StreamWriterBuilder wbuilder;
    wbuilder["indentation"] = "";
    this->BuildRequest(id++, method, parameter, request, isNotification);

    result = Json::writeString(wbuilder, request);
}

void RpcProtocolClient::HandleResponse(const std::string &response,
                                       Json::Value &result)
{ //解析string to json
    Json::Reader reader;
    Json::Value value;

    try
    {
        if (reader.parse(response, value))
        {
            this->HandleResponse(value, result);
        }
        else
        {
            throw RpcException(Errors::ERROR_RPC_JSON_PARSE_ERROR,
                               " " + response);
        }
    }
    catch (Json::Exception &e)
    {
        throw RpcException(Errors::ERROR_RPC_JSON_PARSE_ERROR, " " + response);
    }
}

Json::Value RpcProtocolClient::HandleResponse(const Json::Value &value,
                                              Json::Value &result)
{ //处理响应 取出result
    if (this->ValidateResponse(value))
    {
        if (this->HasError(value))
        {
            this->throwErrorException(value);
        }
        else
        {
            result = value[KEY_RESULT];
        }
    }
    else
    {
        throw RpcException(Errors::ERROR_CLIENT_INVALID_RESPONSE,
                           " " + value.toStyledString());
    }
    return value[KEY_ID];
}

void RpcProtocolClient::BuildRequest(int id, const std::string &method,
                                     const Json::Value &parameter,
                                     Json::Value &result, bool isNotification)
{ // 由函数和参数构建Json
    result[KEY_PROCEDURE_NAME] = method;
    if (parameter != Json::nullValue)
        result[KEY_PARAMETER] = parameter;
    result[KEY_ID] = id;
}

void RpcProtocolClient::throwErrorException(const Json::Value &response)
{
    if (response[KEY_ERROR].isMember(KEY_ERROR_MESSAGE) &&
        response[KEY_ERROR][KEY_ERROR_MESSAGE].isString())
    {
        if (response[KEY_ERROR].isMember(KEY_ERROR_DATA))
        {
            throw RpcException(response[KEY_ERROR][KEY_ERROR_CODE].asInt(),
                               response[KEY_ERROR][KEY_ERROR_MESSAGE].asString(),
                               response[KEY_ERROR][KEY_ERROR_DATA]);
        }
        else
        {
            throw RpcException(response[KEY_ERROR][KEY_ERROR_CODE].asInt(),
                               response[KEY_ERROR][KEY_ERROR_MESSAGE].asString());
        }
    }
    else
    {
        throw RpcException(response[KEY_ERROR][KEY_ERROR_CODE].asInt());
    }
}

bool RpcProtocolClient::ValidateResponse(const Json::Value &response)
{ //判断json是否合法
    if (!response.isObject() || !response.isMember(KEY_ID))
        return false;
    if (response.isMember(KEY_RESULT) && response.isMember(KEY_ERROR))
        return false;
    if (!response.isMember(KEY_RESULT) && !response.isMember(KEY_ERROR))
        return false;
    if (response.isMember(KEY_ERROR) &&
        !(response[KEY_ERROR].isObject() &&
          response[KEY_ERROR].isMember(KEY_ERROR_CODE) &&
          response[KEY_ERROR][KEY_ERROR_CODE].isIntegral()))
        return false;

    return true;
}

bool RpcProtocolClient::HasError(const Json::Value &response)
{
    if (response.isMember(KEY_ERROR))
        return true;
    return false;
}
