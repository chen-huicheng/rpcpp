#include "rpcpp/common/errors.h"
#include "rpcpp/common/rpcexception.h"

using namespace rpcpp;

std::map<int, std::string> Errors::possibleErrors;
Errors::_init Errors::_initializer;

const int Errors::ERROR_RPC_JSON_PARSE_ERROR = -1001;
const int Errors::ERROR_RPC_METHOD_NOT_FOUND = -1002;
const int Errors::ERROR_RPC_INVALID_REQUEST = -1003;
const int Errors::ERROR_RPC_INVALID_PARAMS = -1004;
const int Errors::ERROR_RPC_INTERNAL_ERROR = -1005;

const int Errors::ERROR_SERVER_PROCEDURE_IS_METHOD = -2001;
const int Errors::ERROR_SERVER_PROCEDURE_IS_NOTIFICATION = -2002;
const int Errors::ERROR_SERVER_PROCEDURE_POINTER_IS_NULL = -2003;
const int Errors::ERROR_SERVER_CONNECTOR = -2004;

const int Errors::ERROR_CLIENT_CONNECTOR = -3001;
const int Errors::ERROR_CLIENT_INVALID_RESPONSE = -3002;

Errors::_init::_init()
{
    // Official Errors
    possibleErrors[ERROR_RPC_INVALID_REQUEST] = "INVALID_JSON_REQUEST: The JSON sent is not a valid JSON-RPC Request object";
    possibleErrors[ERROR_RPC_METHOD_NOT_FOUND] = "METHOD_NOT_FOUND: The method being requested is not available on this server";
    possibleErrors[ERROR_RPC_INVALID_PARAMS] = "INVALID_PARAMS: Invalid method parameters (invalid name and/or type) recognised";
    possibleErrors[ERROR_RPC_JSON_PARSE_ERROR] ="JSON_PARSE_ERROR: The JSON-Object is not JSON-Valid";
    possibleErrors[ERROR_RPC_INTERNAL_ERROR] = "INTERNAL_ERROR: ";

    possibleErrors[ERROR_SERVER_PROCEDURE_IS_METHOD] = "PROCEDURE_IS_METHOD: The requested notification is declared as a method";
    possibleErrors[ERROR_SERVER_PROCEDURE_IS_NOTIFICATION] = "PROCEDURE_IS_NOTIFICATION: The requested method is declared as notification";
    possibleErrors[ERROR_SERVER_PROCEDURE_POINTER_IS_NULL] = "PROCEDURE_POINTER_IS_NULL: Server has no function Reference registered";

    possibleErrors[ERROR_CLIENT_INVALID_RESPONSE] = "The response is invalid";
    possibleErrors[ERROR_CLIENT_CONNECTOR] = "Client connector error";
    possibleErrors[ERROR_SERVER_CONNECTOR] = "Server connector error";
}

std::string Errors::GetErrorMessage(int errorCode)
{
    if (possibleErrors.find(errorCode) == possibleErrors.end())
    {
        return "";
    }
    return possibleErrors[errorCode];
}
