//
// Created by frank on 18-1-24.
//

#include <iostream>
#include "echoClientStub.h"
#include "rpcpp/client/RpcClient.h"
#include "rpcpp/client/connectors/LinuxTcpSocketClient.h"
using namespace rpcpp;


int main()
{
    std::string hostname="127.0.0.1";
    unsigned int port=12354;
    LinuxTcpSocketClient connect(hostname,port);
    echoClientStub echoclient(connect);
    std::cout<<echoclient.echo("hello")<<std::endl;
    
}