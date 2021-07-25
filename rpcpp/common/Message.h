
#ifndef RPCPP_COMMON_MESSAGE_H
#define RPCPP_COMMON_MESSAGE_H
#include <string>
#include "rpcpp/common/StreamReader.h"
#include "rpcpp/common/StreamWriter.h"
#include "rpcpp/common/RpcException.h"
namespace rpcpp
{
    class Message
    {
    public:
        static const uint16_t MSG_MAGIC;
        static const uint16_t MSG_MAGICLEN;
        static const uint32_t MSG_BODYLEN;
        static const uint32_t MSG_HEADLEN;

    public:
        Message() : reader(DEFAULT_BUFFER_SIZE){};
        ~Message(){};
        int ReadMessage(int fd, std::string &msg);
        void SendMessage(int fd, const std::string &msg);
        int ReadHeader(int fd);
        int WriteHeader(int fd, int len);

    private:
        StreamWriter writer;
        StreamReader reader;
    };

}
#endif /*RPCPP_COMMON_MESSAGE_H*/