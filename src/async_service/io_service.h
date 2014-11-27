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

        void run();
        void stop();
        void clean_stop();


        ~io_service();

    private:
        friend class async_server;
        friend class async_socket;

        void read_waiter(int, size_t, std::function <void(async_type<async_socket>, async_type<void*>)>);
        void write_waiter(int, void*, size_t, std::function <void(async_type<async_socket>)>);
        void accept_waiter(int, std::function <void(async_type<async_socket>)>);
        void connect_waiter(int, const char*, int, std::function <void(async_type<async_socket>)>);

        bool clean;
        epoll efd;
        volatile int stopper;

        std::map <int, io_events> data;
    };
}

#endif // IO_SERVICE