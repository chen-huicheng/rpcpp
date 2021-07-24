#ifndef RPCPP_COMMON_POLLER_H
#define RPCPP_COMMON_POLLER_H
#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include "rpcpp/common/noncopyable.h"
#include "rpcpp/common/RpcException.h"
namespace rpcpp
{
    class Poller : public noncopyable
    {
    private:
        /* data */
        int epollfd;
        epoll_event *events_;
        const int max_event_number;

    public:
        Poller(int max_event_number = 1000);
        ~Poller();
        int addfd(int fd, bool one_shot);
        int removefd(int fd);
        int wait(std::vector<epoll_event> &events, int timeout);

    private:
        int setnonblocking(int fd);
    };
}

#endif /*RPCPP_COMMON_POLLER_H*/
