
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
        Message():read(false),written(false),firstread(true){};
        ~Message(){};
        void init(int _fd);
        int ReadMessage();
        int SendMessage();
        std::string getMsg(){
            std::string tmp=msg;
            return tmp;
        }
        void setMsg(const std::string &_msg){
            msg.clear();
            HeaderPack(_msg.size());
            msg.append(_msg);
            len=msg.size();
        }
    private:
        int fd;
        bool read,written,firstread;
        std::string msg;
        int len;
        int HeaderUnpack();
        int HeaderPack(int n);
        static StreamWriter writer;
        static StreamReader reader;
    };

}
#endif /*RPCPP_COMMON_MESSAGE_H*/