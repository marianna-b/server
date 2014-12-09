#include <functional>
#include <map>
#include <sys/signalfd.h>
#include "epoll.h"
#include "io_events.h"


#ifndef IO_SERVICE
#define IO_SERVICE

namespace tcp {

    struct io_service {

        io_service();
        io_service(const io_service&) = delete;
        io_service(io_service&&) = default;
        bool operator < (const io_service&) const;

        void run();
        void stop();
        void clean_stop();


        ~io_service();

    private:
        friend class async_server;
        friend class async_socket;
        friend class io_events;

        void read_waiter(async_socket*, size_t, std::function <void(std::string, async_socket*, void*)>);
        void write_waiter(async_socket*, void*, size_t, std::function <void(std::string, async_socket*)>);
        void accept_waiter(int, std::function <void(std::string, async_socket*)>);
        void connect_waiter(async_socket*, const char*, int, std::function <void(std::string, async_socket*)>);

        bool clean;
        epoll efd;
        volatile int stopper;

        std::map <int, io_events> data;
    };
}

#endif // IO_SERVICE