
#include "ServiceStubGenerator.h"

using namespace rpcpp;

namespace
{

    std::string serviceStubTemplate(
        const std::string &macroName,
        const std::string &userClassName,
        const std::string &stubClassName,
        const std::string &serviceName,
        const std::string &stubProcedureBindings,
        const std::string &stubProcedureDefinitions)
    {
        std::string str =
            R"(
/*
 * This stub is generated by RPCPP, DO NOT modify it!
 */
#ifndef RPCPP_[macroName]_H
#define RPCPP_[macroName]_H

#include <jsoncpp/json/json.h>

#include "rpcpp/server/Server.h"

class [userClassName];

namespace rpcpp
{

template <typename S>
class [stubClassName]: noncopyable
{
protected:
    explicit
    [stubClassName](RpcServer& server)
    {
        static_assert(std::is_same_v<S, [userClassName]>,
                      "derived class name should be '[userClassName]'");

        auto service = new RpcService;

        [stubProcedureBindings]

        server.addService("[serviceName]", service);
    }

    ~[stubClassName]() = default;

private:
    [stubProcedureDefinitions]

private:
    S& convert()
    {
        return static_cast<S&>(*this);
    }
};

}

#endif //rpcpp_[macroName]_H
)";

        replaceAll(str, "[macroName]", macroName);
        replaceAll(str, "[userClassName]", userClassName);
        replaceAll(str, "[stubClassName]", stubClassName);
        replaceAll(str, "[serviceName]", serviceName);
        replaceAll(str, "[stubProcedureBindings]", stubProcedureBindings);
        replaceAll(str, "[stubProcedureDefinitions]", stubProcedureDefinitions);
        return str;
    }

    std::string stubProcedureBindTemplate(
        const std::string &procedureName,
        const std::string &stubClassName,
        const std::string &stubProcedureName,
        const std::string &procedureParams)
    {
        std::string str =
            R"(
service->addProcedureReturn("[procedureName]", new ProcedureReturn(
        std::bind(&[stubClassName]::[stubProcedureName], this, _1, _2)
        [procedureParams]
));
)";

        replaceAll(str, "[procedureName]", procedureName);
        replaceAll(str, "[stubClassName]", stubClassName);
        replaceAll(str, "[stubProcedureName]", stubProcedureName);
        replaceAll(str, "[procedureParams]", procedureParams);
        return str;
    }

    std::string stubNotifyBindTemplate(
        const std::string &notifyName,
        const std::string &stubClassName,
        const std::string &stubNotifyName,
        const std::string &notifyParams)
    {
        std::string str =
            R"(
service->addProcedureNotify("[notifyName]", new ProcedureNotify(
        std::bind(&[stubClassName]::[stubNotifyName], this, _1)
        [notifyParams]
));
)";

        replaceAll(str, "[notifyName]", notifyName);
        replaceAll(str, "[stubClassName]", stubClassName);
        replaceAll(str, "[stubNotifyName]", stubNotifyName);
        replaceAll(str, "[notifyParams]", notifyParams);
        return str;
    }

    std::string stubProcedureDefineTemplate(
        const std::string &paramsFromJsonArray,
        const std::string &paramsFromJsonObject,
        const std::string &stubProcedureName,
        const std::string &procedureName,
        const std::string &procedureArgs)
    {
        std::string str =
            R"(void [stubProcedureName](Json::Value& request, const RpcDoneCallback& done)
{
    auto& params = request["params"];

    if (params.isArray()) {
        [paramsFromJsonArray]
        convert().[procedureName]([procedureArgs] UserDoneCallback(request, done));
    }
    else {
        [paramsFromJsonObject]
        convert().[procedureName]([procedureArgs] UserDoneCallback(request, done));
    }
})";

        replaceAll(str, "[paramsFromJsonArray]", paramsFromJsonArray);
        replaceAll(str, "[paramsFromJsonObject]", paramsFromJsonObject);
        replaceAll(str, "[stubProcedureName]", stubProcedureName);
        replaceAll(str, "[procedureName]", procedureName);
        replaceAll(str, "[procedureArgs]", procedureArgs);
        return str;
    }

    std::string stubProcedureDefineTemplate(
        const std::string &stubProcedureName,
        const std::string &procedureName)
    {
        std::string str =
            R"(
void [stubProcedureName](Json::Value& request, const RpcDoneCallback& done)
{
    convert().[procedureName](UserDoneCallback(request, done));
}
)";

        replaceAll(str, "[stubProcedureName]", stubProcedureName);
        replaceAll(str, "[procedureName]", procedureName);
        return str;
    }

    std::string stubNotifyDefineTemplate(
        const std::string &paramsFromJsonArray,
        const std::string &paramsFromJsonObject,
        const std::string &stubNotifyName,
        const std::string &notifyName,
        const std::string &notifyArgs)
    {
        std::string str =
            R"(
void [stubNotifyName](Json::Value& request)
{
    auto& params = request["params"];

    if (params.isArray()) {
        [paramsFromJsonArray]
        convert().[NotifyName]([notifyArgs]);
    }
    else {
        [paramsFromJsonObject]
        convert().[NotifyName]([notifyArgs]);
    }
}
)";

        replaceAll(str, "[notifyName]", notifyName);
        replaceAll(str, "[stubNotifyName]", stubNotifyName);
        replaceAll(str, "[notifyArgs]", notifyArgs);
        replaceAll(str, "[paramsFromJsonArray]", paramsFromJsonArray);
        replaceAll(str, "[paramsFromJsonObject]", paramsFromJsonObject);
        return str;
    }

    std::string stubNotifyDefineTemplate(
        const std::string &stubNotifyName,
        const std::string &notifyName)
    {
        std::string str =
            R"(
void [stubNotifyName](Json::Value& request)
{
    convert().[notifyName]();
}
)";

        replaceAll(str, "[stubNotifyName]", stubNotifyName);
        replaceAll(str, "[notifyName]", notifyName);
        return str;
    }

    std::string argsDefineTemplate(
        const std::string &arg,
        const std::string &index,
        Json::ValueType type)
    {
        std::string str = R"(auto [arg] = params[[index]][method];)";
        std::string method = [=]()
        {

//               nullValue = 0, ///< 'null' value
//   intValue,      ///< signed integer value
//   uintValue,     ///< unsigned integer value
//   realValue,     ///< double value
//   stringValue,   ///< UTF-8 string value
//   booleanValue,  ///< bool value
//   arrayValue,    ///< array value (ordered list)
//   objectValue 
            switch (type)
            {
            case Json::booleanValue:
                return ".getBool()";
            case Json::intValue:
                return ".getInt32()";
            case Json::uintValue:
                return ".getInt64()";
            case Json::stringValue:
                return ".getDouble()";
            case Json::arrayValue:
                return ".getString()";
            case Json::objectValue:
            case Json::realValue:
                return ""; //todo
            default:
                assert(false && "bad value type");
                return "bad type";
            }
        }();
        replaceAll(str, "[arg]", arg);
        replaceAll(str, "[index]", index);
        replaceAll(str, "[method]", method);
        return str;
    }

}

std::string ServiceStubGenerator::genStub()
{
    auto macroName = genMacroName();
    auto userClassName = genUserClassName();
    auto stubClassName = genStubClassName();
    auto &serviceName = serviceinfo.name;

    auto bindings = genStubProcedureBindings();
    bindings.append(genStubNotifyBindings());

    auto definitions = genStubProcedureDefinitions();
    definitions.append(genStubNotifyDefinitions());

    return serviceStubTemplate(macroName,
                               userClassName,
                               stubClassName,
                               serviceName,
                               bindings,
                               definitions);
}

std::string ServiceStubGenerator::genMacroName()
{
    std::string result = serviceinfo.name;
    for (char &c : result)
        c = static_cast<char>(toupper(c));
    return result.append("SERVICESTUB");
}

std::string ServiceStubGenerator::genUserClassName()
{
    return serviceinfo.name + "Service";
}

std::string ServiceStubGenerator::genStubClassName()
{
    return serviceinfo.name + "ServiceStub";
}

std::string ServiceStubGenerator::genStubProcedureBindings()
{
    std::string result;
    for (auto &p : serviceinfo.method)
    {
        auto procedureName = p.name;
        auto stubClassName = genStubClassName();
        auto stubProcedureName = genStubGenericName(p);
        auto procedureParams = genGenericParams(p);

        auto binding = stubProcedureBindTemplate(
            procedureName,
            stubClassName,
            stubProcedureName,
            procedureParams);
        result.append(binding);
        result.append("\n");
    }
    return result;
}

std::string ServiceStubGenerator::genStubProcedureDefinitions()
{
    std::string result;
    for (auto &r : serviceinfo.method)
    {
        auto procedureName = r.name;
        auto stubProcedureName = genStubGenericName(r);

        if (r.params.getSize() > 0)
        {
            auto paramsFromJsonArray = genParamsFromJsonArray(r);
            auto paramsFromJsonObject = genParamsFromJsonObject(r);
            auto procedureArgs = genGenericArgs(r);
            auto define = stubProcedureDefineTemplate(
                paramsFromJsonArray,
                paramsFromJsonObject,
                stubProcedureName,
                procedureName,
                procedureArgs);

            result.append(define);
            result.append("\n");
        }
        else
        {
            auto define = stubProcedureDefineTemplate(
                stubProcedureName,
                procedureName);

            result.append(define);
            result.append("\n");
        }
    }
    return result;
}

std::string ServiceStubGenerator::genStubNotifyBindings()
{
    std::string result;
    for (auto &p : serviceinfo.notification)
    {
        auto notifyName = p.name;
        auto stubClassName = genStubClassName();
        auto stubNotifyName = genStubGenericName(p);
        auto notifyParams = genGenericParams(p);

        auto binding = stubNotifyBindTemplate(
            notifyName,
            stubClassName,
            stubNotifyName,
            notifyParams);
        result.append(binding);
        result.append("\n");
    }
    return result;
}

std::string ServiceStubGenerator::genStubNotifyDefinitions()
{
    std::string result;
    for (auto &r : serviceinfo.notification)
    {
        auto notifyName = r.name;
        auto stubNotifyName = genStubGenericName(r);

        if (r.params.getSize() > 0)
        {
            auto paramsFromJsonArray = genParamsFromJsonArray(r);
            auto paramsFromJsonObject = genParamsFromJsonObject(r);
            auto notifyArgs = genGenericArgs(r);
            auto define = stubNotifyDefineTemplate(
                paramsFromJsonArray,
                paramsFromJsonObject,
                stubNotifyName,
                notifyName,
                notifyArgs);

            result.append(define);
            result.append("\n");
        }
        else
        {
            auto define = stubNotifyDefineTemplate(
                stubNotifyName,
                notifyName);

            result.append(define);
            result.append("\n");
        }
    }
    return result;
}

template <typename Rpc>
std::string ServiceStubGenerator::genStubGenericName(const Rpc &r)
{
    return r.name + "Stub";
}

template <typename Rpc>
std::string ServiceStubGenerator::genGenericParams(const Rpc &r)
{
    std::string result;

    for (auto &m : r.params.getObject())
    {
        std::string field = "\"" + m.key.getString() + "\"";
        std::string type = [&]()
        {
            switch (m.value.getType())
            {
            case Json::TYPE_BOOL:
                return "Json::TYPE_BOOL";
            case Json::TYPE_INT32:
                return "Json::TYPE_INT32";
            case Json::TYPE_INT64:
                return "Json::TYPE_INT64";
            case Json::TYPE_DOUBLE:
                return "Json::TYPE_DOUBLE";
            case Json::TYPE_STRING:
                return "Json::TYPE_STRING";
            case Json::TYPE_OBJECT:
                return "Json::TYPE_OBJECT";
            case Json::TYPE_ARRAY:
                return "Json::TYPE_ARRAY";
            default:
                assert(false && "bad value type");
                return "bad type";
            };
        }();
        result.append(", \n").append(field);
        result.append(", ").append(type);
    }
    return result;
}

template <typename Rpc>
std::string ServiceStubGenerator::genGenericArgs(const Rpc &r)
{
    std::string result;
    for (auto &m : r.params.getObject())
    {
        auto arg = m.key.getString();
        result.append(arg);
        result.append(", ");
    }
    return result;
}

template <typename Rpc>
std::string ServiceStubGenerator::genParamsFromJsonArray(const Rpc &r)
{
    std::string result;
    int index = 0;
    for (auto &m : r.params.getObject())
    {
        std::string line = argsDefineTemplate(
            m.key.getString(),
            std::to_string(index),
            m.value.getType());
        index++;
        result.append(line);
        result.append("\n");
    }
    return result;
}

template <typename Rpc>
std::string ServiceStubGenerator::genParamsFromJsonObject(const Rpc &r)
{
    std::string result;
    for (auto &m : r.params.getObject())
    {
        std::string index = "\"" + m.key.getString() + "\"";
        std::string line = argsDefineTemplate(
            m.key.getString(),
            index,
            m.value.getType());
        result.append(line);
        result.append("\n");
    }
    return result;
}