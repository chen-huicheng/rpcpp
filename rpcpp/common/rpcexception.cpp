

#include "rpcpp/common/rpcexception.h"

using namespace rpcpp;

RpcException::RpcException(int code) : code(code), message(Errors::GetErrorMessage(code))
{
    setWhatMessage();
}

RpcException::RpcException(int code, const std::string &message) : code(code), message(Errors::GetErrorMessage(code))
{
    if (!this->message.empty())
        this->message = this->message + ": ";
    this->message = this->message + message;
    setWhatMessage();
}

RpcException::RpcException(int code, const std::string &message, const Json::Value &data)
    : code(code), message(Errors::GetErrorMessage(code)), data(data)
{
    if (!this->message.empty())
        this->message = this->message + ": ";
    this->message = this->message + message;
    setWhatMessage();
}

RpcException::RpcException(const std::string &message) : code(0), message(message)
{
    setWhatMessage();
}

RpcException::~RpcException() throw() {}

int RpcException::GetCode() const { return code; }

const std::string &RpcException::GetMessage() const { return message; }

const Json::Value &RpcException::GetData() const { return data; }

const char *RpcException::what() const throw()
{
    return whatString.c_str();
}

void RpcException::setWhatMessage()
{
    if (code != 0)
    {
        std::stringstream ss;
        ss << "Exception " << code << " : " << message;
        if (data != Json::nullValue)
            ss << ", data: " << data.toStyledString();
        whatString = ss.str();
    }
    else
    {
        whatString = message;
    }
}
