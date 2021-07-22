

#ifndef RPCPP_CPP_ERRORS_H_
#define RPCPP_CPP_ERRORS_H_

#include <map>
// #include <iostream>
// #include <string.h>
#include <string>

#include <jsoncpp/json/json.h>

namespace rpcpp
{
    class RpcException;

    class Errors
    {
    public:
        static std::string GetErrorMessage(int errorCode);
        static class _init
        {
        public:
            _init();
        } _initializer;

        /**
             * Official RPC
             */
        static const int ERROR_RPC_JSON_PARSE_ERROR;
        static const int ERROR_RPC_METHOD_NOT_FOUND;
        static const int ERROR_RPC_INVALID_REQUEST;
        static const int ERROR_RPC_INVALID_PARAMS;
        static const int ERROR_RPC_INTERNAL_ERROR;

        /**
             * Server Library Errors
             */
        static const int ERROR_SERVER_PROCEDURE_IS_METHOD;
        static const int ERROR_SERVER_PROCEDURE_IS_NOTIFICATION;
        static const int ERROR_SERVER_PROCEDURE_POINTER_IS_NULL;
        static const int ERROR_SERVER_CONNECTOR;

        /**
             * Client Library Errors
             */
        static const int ERROR_CLIENT_CONNECTOR;
        static const int ERROR_CLIENT_INVALID_RESPONSE;

    private:
        static std::map<int, std::string> possibleErrors;
    };
} /* namespace rpcpp */
#endif /* RPCPP_CPP_ERRORS_H_ */
