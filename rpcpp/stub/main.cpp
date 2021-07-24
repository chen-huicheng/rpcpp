//
// Created by frank on 18-1-24.
//

#include <unistd.h>

#include <jsoncpp/json/json.h>
#include <memory>
#include <utility>
#include <fstream>
#include "ServiceStubGenerator.h"
#include "ClientStubGenerator.h"
#include "rpcpp/common/RpcException.h"

using namespace rpcpp;

static void usage()
{
    fprintf(stderr,
            "usage: stub_generator <-c/s> [-o] [-i input]\n");
    exit(1);
}

static void writeToFile(StubGenerator& generator, bool outputToFile)
{
    FILE* output;
    if (!outputToFile) {
        output = stdout;
    }
    else {
        std::string outputFileName = generator.genStubClassName() + ".h";
        output = fopen(outputFileName.c_str(), "w");
        if (output == nullptr) {
            perror("error");
            exit(1);
        }
    }

    auto stubString = generator.genStub();
    fputs(stubString.c_str(), output);
}

static std::shared_ptr<StubGenerator>
makeGenerator(bool serverSide, Json::Value& proto)
{
    if (serverSide)
        return std::shared_ptr<ServiceStubGenerator>(proto);
    else
        return std::shared_ptr<ClientStubGenerator>(proto);
}

static void genStub(const std::string filename, bool serverSide, bool outputToFile)
{

    std::ifstream input(filename.c_str(),std::ios::binary);
    Json::Reader reader;
    Json::Value proto;
    reader.parse(input, proto);

    try {
        auto generator = makeGenerator(serverSide, proto);
        writeToFile(*generator, outputToFile);
    }
    catch (RpcException& e) {
        fprintf(stderr, "input error: %s\n", e.what());
        exit(1);
    }
}

int main(int argc, char** argv)
{
    bool serverSide = false;
    bool clientSide = false;
    bool outputToFile = false;
    std::string inputFileName = nullptr;

    int opt;
    while ((opt = getopt(argc, argv, "csi:o")) != -1) {
        switch (opt) {
            case 'c':
                clientSide = true;
                break;
            case 's':
                serverSide = true;
                break;
            case 'o':
                outputToFile = true;
                break;
            case 'i':
                inputFileName = optarg;
                break;
            default:
                fprintf(stderr, "unknown flag %c\n", opt);
                usage();
        }
    }
    if (!serverSide && !clientSide) {
        serverSide = clientSide = true;
    }

    try {
        if (serverSide) {
            genStub(inputFileName, true, outputToFile);
        }
        if (clientSide) {
            genStub(inputFileName, false, outputToFile);
        }
    }
    catch (RpcException& e) {
        fprintf(stderr, "input error: %s\n", e.what());
        exit(1);
    }
}