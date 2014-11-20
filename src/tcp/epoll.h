#ifndef EPOLL
#define EPOLL

namespace tcp {
    struct epoll {

        epoll();
        epoll(const epoll&) = delete;
        epoll(epoll&&) = default;

        void add(int);
        void remove(int);
        void wait();
        ~epoll();

    private:
        int fd;
    };
}

#endif // EPOLL