//
// Created by frank on 18-1-23.
//

#ifndef rpcpp_STUBGENERATOR_H
#define rpcpp_STUBGENERATOR_H

#include <jsoncpp/json/json.h>

namespace rpcpp
{
    class StubGenerator
    {
    public:
        explicit StubGenerator(Json::Value &proto)
        {
            parseProto(proto);
        }
        virtual ~StubGenerator() = default;

    public:
        virtual std::string genStub() = 0;
        virtual std::string genStubClassName() = 0;

    protected:
        struct RpcMethod
        {
            RpcMethod(const std::string &name_, Json::Value &params_, Json::Value &returns_) : name(name_), params(params_), returns(returns_)
            {
            }

            std::string name;
            mutable Json::Value params;
            mutable Json::Value returns;
        };

        struct RpcNotification
        {
            RpcNotification(const std::string &name_, Json::Value &params_) : name(name_), params(params_)
            {
            }

            std::string name;
            mutable Json::Value params;
        };

        struct ServiceInfo
        {
            std::string classname;
            std::vector<RpcMethod> method;
            std::vector<RpcNotification> notification;
        };

        ServiceInfo serviceinfo;

    private:
        void parseProto(Json::Value &proto);
        void parseRpc(Json::Value &rpc);
        void validateParams(Json::Value &params);
        void validateReturns(Json::Value &returns);
    };

    inline void replaceAll(std::string &str, const std::string &from, const std::string &to)
    {
        while (true)
        {
            size_t i = str.find(from);
            if (i != std::string::npos)
            {
                str.replace(i, from.size(), to);
            }
            else
                return;
        }
    }

}

#endif //rpcpp_STUBGENERATOR_H
