#include "rpcpp/client/connectors/tcpsocketclient.h"

#ifdef _WIN32
#include "windowstcpsocketclient.h"
#else
#include "rpcpp/client/connectors/linuxtcpsocketclient.h"
#endif

using namespace rpcpp;
// using namespace std;

TcpSocketClient::TcpSocketClient(const std::string &ipToConnect,const unsigned int &port)
{
#ifdef _WIN32
    realSocket = new WindowsTcpSocketClient(ipToConnect, port);
#else
    realSocket = new LinuxTcpSocketClient(ipToConnect, port);
#endif
}

TcpSocketClient::~TcpSocketClient() { delete realSocket; }

void TcpSocketClient::SendRPCMessage(const std::string &message,std::string &result)
{
    if (realSocket != NULL)
    {
        realSocket->SendRPCMessage(message, result);
    }
}
