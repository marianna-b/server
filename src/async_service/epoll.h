#include <sys/epoll.h>

#ifndef EPOLL
#define EPOLL

namespace tcp {

    enum epoll_type {
        EPOLL_READ = EPOLLIN,
        EPOLL_WRITE = EPOLLOUT,
        EPOLL_ACCEPT = EPOLLOUT | EPOLLONESHOT,
        EPOLL_CONNECT = EPOLLIN | EPOLLONESHOT
    };

    struct epoll {
        epoll();
        epoll(const epoll&) = delete;
        epoll(epoll&&) = default;

        void add(int, epoll_type);
        void remove(int);

        int wait();
        ~epoll();

        epoll_event events[100];
    private:
        int fd;
        int timeout = 10;
        const int max_events = 100;
    };

}

#endif // EPOLL