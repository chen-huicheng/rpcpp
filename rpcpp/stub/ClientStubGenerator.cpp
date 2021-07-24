//
// Created by frank on 18-1-24.
//

#include "ClientStubGenerator.h"
#include <assert.h>

using namespace rpcpp;

namespace
{

    std::string clientStubTemplate(
        const std::string &macroName,
        const std::string &stubClassName,
        const std::string &procedureDefinitions,
        const std::string &notifyDefinitions)
    {
        std::string str = R"(
/*
 * This stub is generated by jrpc, DO NOT modify it!
 */

#ifndef RPCPP_[macroName]STUB_H
#define RPCPP_[macroName]STUB_H

#include <jsoncpp/json/json.h>
#include <string>
#include "rpcpp/common/noncopyable.h"
#include "rpcpp/client/Client.h"

namespace jrpc
{

    class [stubClassName] : noncopyable
    {
    public:
        [stubClassName](IClientConnector &connector) : client_(connector)
        {
        }

        ~[stubClassName]() = default;

        [procedureDefinitions]

        [notifyDefinitions]

    private:
        Client client_;
    };

}
#endif //RPCPP_[macroName]STUB_H
)";
        replaceAll(str, "[macroName]", macroName);
        replaceAll(str, "[stubClassName]", stubClassName);
        replaceAll(str, "[procedureDefinitions]", procedureDefinitions);
        replaceAll(str, "[notifyDefinitions]", notifyDefinitions);
        return str;
    }

    std::string procedureDefineTemplate(
        const std::string &returntype,
        const std::string &procedureName,
        const std::string &procedureArgs,
        const std::string &paramMembers,
        const std::string &returnmethod)
    {

        std::string str = R"(
        [returntype] [procedureName]([procedureArgs])
        {
            Json::Value params,result;
            [paramMembers]

            client.CallMethod("[procedureName]",params,result);
            return result.[returnmethod];
        }
        )";
        replaceAll(str, "[procedureName]", procedureName);
        replaceAll(str, "[procedureArgs]", procedureArgs);
        replaceAll(str, "[paramMembers]", paramMembers);
        replaceAll(str, "[returntype]", returntype);
        replaceAll(str, "[returnmethod]", returnmethod);
        return str;
    }

    std::string notifyDefineTemplate(
        const std::string &notifyName,
        const std::string &notifyArgs,
        const std::string &paramMembers)
    {
        std::string str = R"(
        void [procedureName]([procedureArgs])
        {
            Json::Value params;
            [paramMembers]

            client.CallNotification("[procedureName]",params);
        }
        )";
        replaceAll(str, "[notifyName]", notifyName);
        replaceAll(str, "[notifyArgs]", notifyArgs);
        replaceAll(str, "[paramMembers]", paramMembers);
        return str;
    }

    std::string paramMemberTemplate(const std::string &paramName)
    {
        std::string str = R"(params["[paramName]"]=[paramName];)";
        replaceAll(str, "[paramName]", paramName);
        return str;
    }
    std::string returnMethod(Json::ValueType type)
    {
        switch (type)
        {
        case Json::booleanValue:
            return ".asBool()";
        case Json::intValue:
            return ".asInt()";
        case Json::uintValue:
            return ".asUInt()";
        case Json::stringValue:
            return ".asString()";
        case Json::realValue:
            return "asDouble()";
        case Json::arrayValue:
        case Json::objectValue:
            return "";
        default:
            assert(false && "bad value type");
            return "bad type";
        }
    }
    std::string returnType(Json::ValueType type)
    {
        switch (type)
        {
        case Json::booleanValue:
            return "bool";
        case Json::intValue:
            return "int";
        case Json::uintValue:
            return "uint";
        case Json::stringValue:
            return "std::string";
        case Json::realValue:
            return "double";
        case Json::arrayValue:
        case Json::objectValue:
            return "void"; //TODO:
        default:
            assert(false && "bad value type");
            return "bad type";
        }
    }
    std::string argTemplate(const std::string &argName, Json::ValueType argType)
    {
        std::string str = R"([argType] [argName])";
        std::string typeStr = returnType(argType);
        replaceAll(str, "[argType]", typeStr);
        replaceAll(str, "[argName]", argName);
        return str;
    }

}

std::string ClientStubGenerator::genStub()
{
    auto macroName = genMacroName();
    auto stubClassName = genStubClassName();
    auto procedureDefinitions = genProcedureDefinitions();
    auto notifyDefinitions = genNotifyDefinitions();

    return clientStubTemplate(macroName,
                              stubClassName,
                              procedureDefinitions,
                              notifyDefinitions);
}

std::string ClientStubGenerator::genMacroName()
{
    std::string result = serviceinfo.classname;
    for (char &c : result)
        c = static_cast<char>(toupper(c));
    return result + "CLIENTSTUB";
}

std::string ClientStubGenerator::genStubClassName()
{
    return serviceinfo.classname + "ClientStub";
}

std::string ClientStubGenerator::genProcedureDefinitions()
{
    std::string result;

    auto &serviceName = serviceinfo.classname;

    for (auto &r : serviceinfo.method)
    {
        auto &procedureName = r.name;
        auto procedureArgs = genGenericArgs(r);
        auto paramMembers = genGenericParamMembers(r);
        auto returntype = returnType(r.returns.type());
        auto returnmethod = returnMethod(r.returns.type());
        auto str = procedureDefineTemplate(
            returntype,
            procedureName,
            procedureArgs,
            paramMembers,
            returnmethod);
        result.append(str);
    }
    return result;
}

std::string ClientStubGenerator::genNotifyDefinitions()
{
    std::string result;

    for (auto &r : serviceinfo.notification)
    {
        auto &notifyName = r.name;
        auto notifyArgs = genGenericArgs(r);
        auto paramMembers = genGenericParamMembers(r);

        auto str = notifyDefineTemplate(
            notifyName,
            notifyArgs,
            paramMembers);
        result.append(str);
    }
    return result;
}

template <typename Rpc>
std::string ClientStubGenerator::genGenericArgs(const Rpc &r)
{
    std::string result;
    Json::Value::Members mem = r.params.getMemberNames();
    for (std::string &key : mem)
    {
        std::string one = argTemplate(key,r.params[key].type());
        result.append(", ");
        result.append(one);
    }
    if(!result.empty())
        result.pop_back();
    return result;
}

template <typename Rpc>
std::string ClientStubGenerator::genGenericParamMembers(const Rpc &r)
{
    std::string result;
    Json::Value::Members mem = r.params.getMemberNames();
    for (std::string &key : mem)
    {
        std::string one = paramMemberTemplate(key);
        result.append(one);
    }
    return result;
}