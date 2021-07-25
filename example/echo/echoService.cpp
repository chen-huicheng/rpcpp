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
    echoServer(RpcServer &server):echoServiceStub<echoServer>(server){}
    std::string  echo(std::string message){
        std::cout<<"helo"<<message<<std::endl;
        return "message";
    }
};
int main(){
    std::string hostname="127.0.0.1";
    unsigned int port=12354;
    LinuxTcpSocketServer connect(hostname,port);
    RpcServer rpcserver(connect);
    echoServer echoserver(rpcserver);
    rpcserver.StartListening();
    getchar();
    rpcserver.StopListening();
}