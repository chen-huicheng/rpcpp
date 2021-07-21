
#include "rpcprotocolserver.h"
#include <iostream>
#include "rpcpp/common/errors.h"

using namespace std;
using namespace rpcpp;

void RpcProtocolServer::HandleJsonRequest(const Json::Value &req,
                                            Json::Value &response)
{
    // It could be a Batch Request
    if (req.isArray())
    {
        this->HandleBatchRequest(req, response);
    } // It could be a simple Request
    else if (req.isObject())
    {
        this->HandleSingleRequest(req, response);
    }
    else
    {
        this->WrapError(Json::nullValue, Errors::ERROR_RPC_INVALID_REQUEST,
                        Errors::GetErrorMessage(Errors::ERROR_RPC_INVALID_REQUEST),
                        response);
    }
}
void RpcProtocolServer::HandleSingleRequest(const Json::Value &req,
                                              Json::Value &response)
{
    int error = this->ValidateRequest(req);
    if (error == 0)
    {
        try
        {
            this->ProcessRequest(req, response);
        }
        catch (const RpcException &exc)
        {
            this->WrapException(req, exc, response);
        }
    }
    else
    {
        this->WrapError(req, error, Errors::GetErrorMessage(error), response);
    }
}
void RpcProtocolServer::HandleBatchRequest(const Json::Value &req,
                                             Json::Value &response)
{
    if (req.empty())
        this->WrapError(Json::nullValue, Errors::ERROR_RPC_INVALID_REQUEST,
                        Errors::GetErrorMessage(Errors::ERROR_RPC_INVALID_REQUEST),
                        response);
    else
    {
        for (unsigned int i = 0; i < req.size(); i++)
        {
            Json::Value result;
            this->HandleSingleRequest(req[i], result);
            if (result != Json::nullValue)
                response.append(result);
        }
    }
}
bool RpcProtocolServer::ValidateRequestFields(const Json::Value &request)
{
    if (!request.isObject())
        return false;
    if (!(request.isMember(KEY_REQUEST_METHODNAME) &&
          request[KEY_REQUEST_METHODNAME].isString()))
        return false;
    if (!(request.isMember(KEY_REQUEST_VERSION) &&
          request[KEY_REQUEST_VERSION].isString() &&
          request[KEY_REQUEST_VERSION].asString() == JSON_RPC_VERSION2))
        return false;
    if (request.isMember(KEY_REQUEST_ID) &&
        !(request[KEY_REQUEST_ID].isIntegral() ||
          request[KEY_REQUEST_ID].isString() || request[KEY_REQUEST_ID].isNull()))
        return false;
    if (request.isMember(KEY_REQUEST_PARAMETERS) &&
        !(request[KEY_REQUEST_PARAMETERS].isObject() ||
          request[KEY_REQUEST_PARAMETERS].isArray() ||
          request[KEY_REQUEST_PARAMETERS].isNull()))
        return false;
    return true;
}

void RpcProtocolServer::WrapResult(const Json::Value &request,
                                     Json::Value &response,
                                     Json::Value &result)
{
    response[KEY_REQUEST_VERSION] = JSON_RPC_VERSION2;
    response[KEY_RESPONSE_RESULT] = result;
    response[KEY_REQUEST_ID] = request[KEY_REQUEST_ID];
}

void RpcProtocolServer::WrapError(const Json::Value &request, int code,
                                    const string &message,
                                    Json::Value &result)
{
    result["jsonrpc"] = "2.0";
    result["error"]["code"] = code;
    result["error"]["message"] = message;

    if (request.isObject() && request.isMember("id") &&
        (request["id"].isNull() || request["id"].isIntegral() ||
         request["id"].isString()))
    {
        result["id"] = request["id"];
    }
    else
    {
        result["id"] = Json::nullValue;
    }
}

void RpcProtocolServer::WrapException(const Json::Value &request,
                                        const RpcException &exception,
                                        Json::Value &result)
{
    this->WrapError(request, exception.GetCode(), exception.GetMessage(), result);
    result["error"]["data"] = exception.GetData();
}

procedure_t RpcProtocolServer::GetRequestType(const Json::Value &request)
{
    if (request.isMember(KEY_REQUEST_ID))
        return RPC_METHOD;
    return RPC_NOTIFICATION;
}
