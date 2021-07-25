// #ifndef RPCPP_CPP_WINDOWSTCPSOCKETCLIENT_H_
// #define RPCPP_CPP_WINDOWSTCPSOCKETCLIENT_H_

// #include "rpcpp/common/RpcException.h"
// #include <string>
// #include "rpcpp/client/IClientConnector.h"
// #include <winsock2.h>

// namespace rpcpp
// {
//     class WindowsTcpSocketClient : public IClientConnector
//     {
//         public:
//             WindowsTcpSocketClient(const std::string& hostToConnect, const unsigned int &port);
//             virtual ~WindowsTcpSocketClient();
//             virtual void SendRPCMessage(const std::string& message, std::string& result) ;

//         private:
//             std::string hostToConnect;
//             unsigned int port;
//             static std::string GetErrorMessage(const int &e);
//             SOCKET Connect() ;
//             SOCKET Connect(const std::string& ip, const int& port) ;
//             bool IsIpv4Address(const std::string& ip);
//     };

// }
// #endif /* RPCPP_CPP_WINDOWSTCPSOCKETCLIENT_H_ */
