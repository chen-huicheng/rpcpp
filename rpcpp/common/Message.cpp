#include "rpcpp/common/Message.h"
#include <netdb.h>
#include <string.h>
#include<iostream>
using namespace rpcpp;
const uint16_t Message::MSG_MAGIC = 0x8864;
const uint16_t Message::MSG_MAGICLEN = sizeof(uint16_t);
const uint32_t Message::MSG_BODYLEN = sizeof(uint32_t);
const uint32_t Message::MSG_HEADLEN = MSG_MAGICLEN + MSG_BODYLEN;
StreamWriter Message::writer;
StreamReader Message::reader(DEFAULT_BUFFER_SIZE);
void Message::init(int _fd){
    fd=_fd;
    msg.clear();
    len=0;
    read=written=false;
    firstread=true;
}
int Message::ReadMessage()
{
    if (!read)
    {
        if(firstread){
            msg.clear();
            firstread=false;
        }
        // std::cout<<"ReadMessage"<<std::endl;
        if (reader.Read(fd, msg, MSG_HEADLEN-msg.size()))
        {
            len=HeaderUnpack();
            msg.clear();
            read=true;
        }
        else{
            return -1;
        }
    }
    if (!reader.Read(fd, msg, len-msg.size()))
    {
        return -1;
    }
    read=false;
    firstread=true;
    return 0;
}
int Message::SendMessage()
{
    // std::cout<<"SendMessage"<<std::endl;
    if(!writer.Write(fd, msg))
        return -1;
    return 0;
}

int Message::HeaderUnpack()
{
    const char *buf = msg.c_str();
    uint16_t magic = ntohs(*(uint16_t *)(buf));
    if (magic != MSG_MAGIC)
    {
        return -1;
    }
    int len = ntohl(*(int *)(buf + MSG_MAGICLEN));
    return len;
}
int Message::HeaderPack(int n)
{
    char buf[MSG_HEADLEN + 5];
    uint16_t magic = htons(MSG_MAGIC);
    int len = htonl(n);
    memcpy(buf, &magic, MSG_MAGICLEN);
    memcpy(buf + MSG_MAGICLEN, &len, MSG_BODYLEN);
    buf[MSG_HEADLEN] = '\0';
    for (int i = 0; i < static_cast<int>(MSG_HEADLEN); ++i)
        msg.push_back(buf[i]);
}