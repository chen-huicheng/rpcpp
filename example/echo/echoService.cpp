#include <iostream>
#include <string>
#include <jsoncpp/json/json.h>
#include "echoServiceStub.h"
#include "rpcpp/server/RpcServer.h"
#include "rpcpp/server/connectors/LinuxTcpSocketServer.h"
using namespace rpcpp;
class echoServer : public echoServiceStub<echoServer>
{
public:
    echoServer(RpcServer &server):echoServiceStub(server){}
    std::string  echo(std::string message){
        return message;
    }
    int add(int a,int b){
        return a+b;
    }
};
int main(){
    unsigned int port=12354;
    LinuxTcpSocketServer connect(port);
    RpcServer rpcserver(connect);
    echoServer echoserver(rpcserver);
    rpcserver.StartListening();
    getchar();
    rpcserver.StopListening();
}