#include <sys/epoll.h>

#ifndef EPOLL
#define EPOLL

namespace tcp {

    enum epoll_type {
        EPOLL_READ = EPOLLIN,
        EPOLL_WRITE = EPOLLOUT,
        EPOLL_BOTH = EPOLLOUT | EPOLLIN,
    };

    struct epoll {
        epoll();
        epoll(const epoll&) = default;
        epoll(epoll&&) = default;

        epoll& operator=(const epoll&) = default;

        void add(int, epoll_type);
        void remove(int);
        void modify(int, epoll_type);
        void close();
        int wait();
        ~epoll();

        epoll_event events[100];
    private:
        int fd;
        int timeout = 10;
        static const int MAX_EVENTS = 100;
    };

}

#endif // EPOLL