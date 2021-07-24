
#include "rpcpp/server/RpcProtocolServer.h"
#include "rpcpp/common/Errors.h"
#include <iostream>

using namespace rpcpp;

const std::string RpcProtocolServer::KEY_REQUEST_METHODNAME = "method";
const std::string RpcProtocolServer::KEY_REQUEST_ID = "id";
const std::string RpcProtocolServer::KEY_REQUEST_PARAMETERS = "params";
const std::string RpcProtocolServer::KEY_RESPONSE_RESULT = "result";
const std::string RpcProtocolServer::KEY_ERROR = "error";
const std::string RpcProtocolServer::KEY_ERROR_CODE = "code";
const std::string RpcProtocolServer::KEY_ERROR_MESSAGE = "message";
const std::string RpcProtocolServer::KEY_ERROR_DATA = "data";

Json::Value RpcProtocolServer::HandleRequest(const std::string &request)
{
    Json::Value result;
    HandleRequest(request, result);
    return result;
}
void RpcProtocolServer::HandleRequest(const std::string &request, Json::Value &result)
{
    Json::Reader reader;
    try
    {
        if (reader.parse(request, result, false))
        {
            if (!ValidateRequest(result))
            {
                WrapError(Json::nullValue, Errors::ERROR_RPC_INVALID_REQUEST, Errors::GetErrorMessage(Errors::ERROR_RPC_INVALID_REQUEST), result);
            }
        }
        else
        {
            WrapError(Json::nullValue, Errors::ERROR_RPC_JSON_PARSE_ERROR, Errors::GetErrorMessage(Errors::ERROR_RPC_JSON_PARSE_ERROR), result);
        }
    }
    catch (const Json::Exception &e)
    {
        WrapError(Json::nullValue, Errors::ERROR_RPC_JSON_PARSE_ERROR, Errors::GetErrorMessage(Errors::ERROR_RPC_JSON_PARSE_ERROR), result);
    }
}

void RpcProtocolServer::BuildResponse(const Json::Value &request, const Json::Value &result, std::string &response)
{
    Json::Value jresponse;
    Json::StreamWriterBuilder wbuilder;
    jresponse[KEY_REQUEST_ID] = request[KEY_REQUEST_ID];
    jresponse[KEY_RESPONSE_RESULT] = result;
    if(result.isObject()&&result.isMember(KEY_ERROR)){
        jresponse = result;
    }
        
    response = Json::writeString(wbuilder, jresponse);
}

std::string RpcProtocolServer::BuildResponse(const Json::Value &request, const Json::Value &result)
{
    std::string response;
    BuildResponse(request,result,response);
    return response;
}

bool RpcProtocolServer::ValidateRequest(const Json::Value &request)
{
    if (!request.isObject())
        return false;
    if (!(request.isMember(KEY_REQUEST_METHODNAME) && request[KEY_REQUEST_METHODNAME].isString()))
        return false;
    if (request.isMember(KEY_REQUEST_ID) && !request[KEY_REQUEST_ID].isIntegral())
        return false;
    if (request.isMember(KEY_REQUEST_PARAMETERS) && !request[KEY_REQUEST_PARAMETERS].isObject())
        return false;
    return true;
}

void RpcProtocolServer::WrapResult(const Json::Value &request, Json::Value &response, Json::Value &result)
{
    response[KEY_RESPONSE_RESULT] = result;
    response[KEY_REQUEST_ID] = request[KEY_REQUEST_ID];
}

void RpcProtocolServer::WrapError(const Json::Value &request, int code, const std::string &message, Json::Value &result)
{
    result[KEY_ERROR][KEY_ERROR_CODE] = code;
    result[KEY_ERROR][KEY_ERROR_MESSAGE] = message;

    if (request.isObject() && request.isMember(KEY_REQUEST_ID) &&
        (request[KEY_REQUEST_ID].isNull() || request[KEY_REQUEST_ID].isIntegral() ||
         request[KEY_REQUEST_ID].isString()))
    {
        result[KEY_REQUEST_ID] = request[KEY_REQUEST_ID];
    }
    else
    {
        result[KEY_REQUEST_ID] = Json::nullValue;
    }
}

void RpcProtocolServer::WrapException(const Json::Value &request, const RpcException &exception, Json::Value &result)
{
    WrapError(request, exception.GetCode(), exception.GetMessage(), result);
    result[KEY_ERROR][KEY_ERROR_DATA] = exception.GetData();
}
