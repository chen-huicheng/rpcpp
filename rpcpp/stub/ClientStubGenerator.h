//
// Created by frank on 18-1-24.
//

#ifndef rpcpp_CLIENTSTUBGENERATOR_H
#define rpcpp_CLIENTSTUBGENERATOR_H

#include "StubGenerator.h"

namespace rpcpp
{

    class ClientStubGenerator : public StubGenerator
    {
    public:
        explicit ClientStubGenerator(json::Value &proto) : StubGenerator(proto)
        {
        }

        std::string genStub() override;
        std::string genStubClassName() override;

    private:
        std::string genMacroName();

        std::string genProcedureDefinitions();
        std::string genNotifyDefinitions();

        template <typename Rpc>
        std::string genGenericArgs(const Rpc &r, bool appendComma);
        template <typename Rpc>
        std::string genGenericParamMembers(const Rpc &r);
    };

}

#endif //rpcpp_CLIENTSTUBGENERATOR_H
