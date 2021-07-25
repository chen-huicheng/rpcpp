#include "Poller.h"

using namespace rpcpp;
Poller::Poller(int max_event_number):max_event_number(max_event_number)
{
    events_ = new epoll_event[max_event_number];
    epollfd = epoll_create(5);
}

Poller::~Poller()
{
    delete[] events_;
}
int Poller::addfd(int fd, bool one_shot) {
    epoll_event event;
    event.data.fd = fd;

    event.events = EPOLLIN | EPOLLRDHUP;

    if (one_shot)
        event.events |= EPOLLONESHOT;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    return setnonblocking(fd);
}
int Poller::removefd(int fd) {
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, 0);
    return close(fd);
}
int Poller::wait(std::vector<epoll_event> &events,int timeout) {
    int number = epoll_wait(epollfd, events_, max_event_number, timeout);
    if(number<0)
        return number;
    for(int i=0;i<number;i++)
        events.push_back(events_[i]);
    return number;
}

int Poller::setnonblocking(int fd) {
    int old_option = fcntl(fd, F_GETFL, 0);
    int new_option = old_option | O_NONBLOCK;
    if(fcntl(fd, F_SETFL, new_option)!=0){
        return -1;
    }
    return old_option;
}