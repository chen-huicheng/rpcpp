//
// Created by frank on 18-1-23.
//

#include <unordered_set>

#include "StubGenerator.h"
#include "rpcpp/common/rpcexception.h"

using namespace rpcpp;

namespace
{

    void expect(bool result, const char *errMsg)
    {
        if (!result)
        {
            throw RpcException(errMsg);
        }
    }

}

void StubGenerator::parseProto(Json::Value &proto)
{
    expect(proto.isObject(),
           "expect object");
    expect(proto.isMember("name") == 2,
           "expect 'name' fields in object");

    auto name = proto["name"];

    expect(name != proto.memberEnd(),
           "missing service name");
    expect(name->value.isString(),
           "service name must be string");
    serviceInfo_.name = name->value.getString();

    auto rpc = proto.findMember("rpc");
    expect(rpc != proto.memberEnd(),
           "missing service rpc definition");
    expect(rpc->value.isArray(),
           "rpc field must be array");

    size_t n = rpc->value.getSize();
    for (size_t i = 0; i < n; i++)
    {
        parseRpc(rpc->value[i]);
    }
}

void StubGenerator::parseRpc(Json::Value &rpc)
{
    expect(rpc.isObject(),
           "rpc definition must be object");

    auto name = rpc.findMember("name");
    expect(name != rpc.memberEnd(),
           "missing name in rpc definition");
    expect(name->value.isString(),
           "rpc name must be string");

    auto params = rpc.findMember("params");
    bool hasParams = params != rpc.memberEnd();
    if (hasParams)
    {
        validateParams(params->value);
    }

    auto returns = rpc.findMember("returns");
    bool hasReturns = returns != rpc.memberEnd();
    if (hasReturns)
    {
        validateReturns(returns->value);
    }

    auto paramsValue = hasParams ? params->value : Json::Value(Json::TYPE_OBJECT);

    if (hasReturns)
    {
        RpcMethod r(name->value.getString(), paramsValue, returns->value);
        serviceInfo_.rpcReturn.push_back(r);
    }
    else
    {
        RpcNotification r(name->value.getString(), paramsValue);
        serviceInfo_.rpcNotify.push_back(r);
    }
}

void StubGenerator::validateParams(Json::Value &params)
{
    std::unordered_set<std::string_view> set;

    for (auto &p : params.getObject())
    {

        auto key = p.key.getStringView();
        auto unique = set.insert(key).second;
        expect(unique, "duplicate param name");

        switch (p.value.getType())
        {
        case Json::TYPE_NULL:
            expect(false, "bad param type");
            break;
        default:
            break;
        }
    }
}

void StubGenerator::validateReturns(Json::Value &returns)
{
    switch (returns.getType())
    {
    case Json::TYPE_NULL:
    case Json::TYPE_ARRAY:
        expect(false, "bad returns type");
        break;
    case Json::TYPE_OBJECT:
        validateParams(returns);
        break;
    default:
        break;
    }
}