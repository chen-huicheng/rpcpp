

#include "rpcpp/common/exception.h"

using namespace rpcpp;

RpcException::RpcException(int code)
    : code(code), message(Errors::GetErrorMessage(code))
{
    this->setWhatMessage();
}

RpcException::RpcException(int code, const std::string &message)
    : code(code), message(Errors::GetErrorMessage(code))
{
    if (!this->message.empty())
        this->message = this->message + ": ";
    this->message = this->message + message;
    this->setWhatMessage();
}

RpcException::RpcException(int code, const std::string &message,
                           const Json::Value &data)
    : code(code), message(Errors::GetErrorMessage(code)), data(data)
{
    if (!this->message.empty())
        this->message = this->message + ": ";
    this->message = this->message + message;
    this->setWhatMessage();
}

RpcException::RpcException(const std::string &message)
    : code(0), message(message)
{
    this->setWhatMessage();
}

RpcException::~RpcException() throw() {}

int RpcException::GetCode() const { return code; }

const std::string &RpcException::GetMessage() const { return message; }

const Json::Value &RpcException::GetData() const { return data; }

const char *RpcException::what() const throw()
{
    return this->whatString.c_str();
}

void RpcException::setWhatMessage()
{
    if (this->code != 0)
    {
        std::stringstream ss;
        ss << "Exception " << this->code << " : " << this->message;
        if (data != Json::nullValue)
            ss << ", data: " << data.toStyledString();
        this->whatString = ss.str();
    }
    else
    {
        this->whatString = this->message;
    }
}
