#ifndef JSONRPC_CPP_SERVERCONNECTOR_H_
#define JSONRPC_CPP_SERVERCONNECTOR_H_

#include <string>
#include <functional>

namespace rpcpp
{

    class AbstractServerConnector
    {
    public:
        AbstractServerConnector();
        virtual ~AbstractServerConnector();

        virtual bool StartListening() = 0;
        virtual bool StopListening() = 0;

        void SetHandler(const std::function<void(const std::string &request, std::string &response)> &cb);
    protected:
        void ProcessRequest(const std::string &request,std::string &response);

    private:
        std::function<void(const std::string &request, std::string &response)> handler;
    };

} /* namespace rpcpp */
#endif /* JSONRPC_CPP_ERVERCONNECTOR_H_ */
