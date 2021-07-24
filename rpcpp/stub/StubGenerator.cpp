#include <unordered_set>

#include "StubGenerator.h"
#include "rpcpp/common/RpcException.h"

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
    expect(proto.isObject(), "expect object");
    expect(proto.isMember("classname"), "missing name in userclass definition");
    expect(proto["classname"].type() == Json::stringValue, "userclass name must be string");

    serviceinfo.classname = proto["classname"].asString();
    expect(proto["rpc"].type() == Json::arrayValue, "rpc must be array");
    for (unsigned int i = 0; i < proto["rpc"].size(); i++)
    {
        parseRpc(proto["rpc"][i]);
    }
}

void StubGenerator::parseRpc(Json::Value &method)
{
    expect(method.isObject(), "method definition must be object");

    expect(method.isMember("name"), "missing name in method definition");

    expect(method["name"].isString(), "rpc name must be string");

    std::string name = method["name"].asString();
    Json::Value params = Json::nullValue, returns = Json::nullValue;
    if (method.isMember("params"))
    {
        expect(method["params"].isObject(), "params must be object");
        params = method["params"];
        validateParams(params);
    }

    if (method.isMember("returns"))
    {
        returns = method["returns"];
        validateReturns(returns);
    }
    if (returns.type() != Json::nullValue)
    {
        serviceinfo.method.push_back(RpcMethod(name, params, returns));
    }
    else
    {
        serviceinfo.notification.push_back(RpcNotification(name, params));
    }
}

void StubGenerator::validateParams(Json::Value &params)
{
    std::unordered_set<std::string> params_set;
    Json::Value::Members mem = params.getMemberNames();
    for (auto key : mem)
    {
        auto unique = params_set.insert(key).second;
        expect(unique, "duplicate param name");
        switch (params[key].type())
        {
        case Json::nullValue:
            expect(false, "bad param type");
            break;
        default:
            break;
        }
    }
}

void StubGenerator::validateReturns(Json::Value &returns)
{
    switch (returns.type())
    {
    case Json::nullValue:
    case Json::arrayValue:
        expect(false, "bad returns type");
        break;
    case Json::objectValue:
        validateParams(returns);
        break;
    default:
        break;
    }
}