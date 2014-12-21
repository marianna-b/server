
#ifndef IO_SERVICE
#define IO_SERVICE

#include <functional>
#include <map>
#include <sys/signalfd.h>
#include "epoll.h"
#include "io_events.h"


namespace tcp {

    struct io_service {

        io_service();
        io_service(const io_service&) = delete;
        io_service(io_service&&) = default;
        bool operator < (const io_service&) const;

        void run();
        void stop();
        void pause();


        ~io_service();

    private:
        friend class async_server;
        friend class async_socket;
        friend class io_events;
        friend class signal_handler;

        void read_waiter(async_socket*, size_t, std::function <void(int, async_socket*, size_t, void*)>);
        void read_some_waiter(async_socket*, size_t, std::function <void(int, async_socket*, size_t, void*)>);
        void write_waiter(async_socket*, void*, size_t, std::function <void(int, async_socket*)>);
        void accept_waiter(struct async_server*, std::function <void(int, async_socket*)>);
        void connect_waiter(async_socket*, const char*, int, std::function <void(int, async_socket*)>);

        bool clean;
        epoll* efd;
        int stopper;
        int pause_fd;

        std::map <int, io_events> data;
    };
}

#endif // IO_SERVICE