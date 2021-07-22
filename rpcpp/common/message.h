
#ifndef RPCPP_COMMON_MESSAGE_H
#define RPCPP_COMMON_MESSAGE_H
#include <string>
#include "rpcpp/common/streamreader.h"
#include "rpcpp/common/streamwriter.h"
#include "rpcpp/common/rpcexception.h"
namespace rpcpp
{
    class Message
    {
    public:
        static const uint16_t MSG_MAGIC;
        static const uint16_t MSG_MAGICLEN;
        static const int MSG_BODYLEN;
        static const int MSG_HEADLEN;

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