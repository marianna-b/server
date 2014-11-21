#include <sys/epoll.h>

#ifndef EPOLL
#define EPOLL

namespace tcp {
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

    enum epoll_type {
        EPOLL_READ,
        EPOLL_WRITE,
        EPOLL_ONESHOT
    };
}

#endif // EPOLL