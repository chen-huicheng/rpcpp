#include "rpcpp/common/message.h"
#include <netdb.h>
#include <string.h>
using namespace rpcpp;
const uint16_t Message::MSG_MAGIC = 0x8864;
const uint16_t Message::MSG_MAGICLEN = sizeof(uint16_t);
const int Message::MSG_BODYLEN = sizeof(int);
const int Message::MSG_HEADLEN = MSG_MAGICLEN + MSG_BODYLEN;

int Message::ReadMessage(int fd, std::string &msg)
{
    int len = ReadHeader(fd);
    if (len < 0)
    {
        throw RpcException(Errors::ERROR_CLIENT_CONNECTOR, "RPC header error");
    }
    if (!reader.Read(fd, msg, len))
    {
        throw RpcException(Errors::ERROR_CLIENT_CONNECTOR, "Could not write request");
    }
}
void Message::SendMessage(int fd, const std::string &msg)
{
    int rt = WriteHeader(fd, msg.size());
    if (rt < 0)
    {
        throw RpcException(Errors::ERROR_CLIENT_CONNECTOR, "RPC header error");
    }
    if (!writer.Write(fd, msg))
    {
        throw RpcException(Errors::ERROR_CLIENT_CONNECTOR, "Could not write request");
    }
}

int Message::ReadHeader(int fd)
{
    std::string target;
    reader.Read(fd, target, MSG_HEADLEN);
    const char *buf = target.c_str();
    uint16_t magic = ntohs(*(uint16_t *)(buf));
    if (magic != MSG_MAGIC)
    {
        return -1;
    }
    int len = ntohl(*(int *)(buf + MSG_MAGICLEN));
    return len;
}
int Message::WriteHeader(int fd, int n)
{
    char buf[MSG_HEADLEN + 5];
    uint16_t magic = htons(MSG_MAGIC);
    int len = htonl(n);
    memcpy(buf, &magic, MSG_MAGICLEN);
    memcpy(buf + MSG_MAGICLEN, &len, MSG_BODYLEN);
    buf[MSG_HEADLEN] = '\0';
    std::string target(buf);
    writer.Write(fd, target);
}