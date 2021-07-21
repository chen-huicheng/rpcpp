
#ifndef RPCPP_CPP_EXCEPTION_H_
#define RPCPP_CPP_EXCEPTION_H_

#include <sstream>
#include <string>
#include <exception>
#include <jsoncpp/json/json.h>
#include "rpcpp/common/errors.h"

namespace rpcpp
{
    class RpcException : public std::exception
    {
    public:
        RpcException(int code);
        RpcException(int code, const std::string &message);
        RpcException(int code, const std::string &message, const Json::Value &data);
        RpcException(const std::string &message);

        virtual ~RpcException() throw();

        int GetCode() const;
        const std::string &GetMessage() const;
        const Json::Value &GetData() const;

        virtual const char *what() const throw();

    private:
        int code;
        std::string message;
        std::string whatString;
        Json::Value data;
        void setWhatMessage();
    };

} /* namespace rpcpp */
#endif /* RPCPP_CPP_EXCEPTION_H_ */
