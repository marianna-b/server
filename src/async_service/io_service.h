#include <functional>
#include <tcp/tcp_socket.h>
#include <map>
#include "epoll.h"

#ifndef IO_SERVICE
#define IO_SERVICE

namespace tcp {

    struct read_buffer {
        char* buf;
        size_t needed;
        size_t done;

        read_buffer() = default; // TODO throw logic error
        read_buffer(char*, size_t);
    };

    struct write_buffer {
        const char* buf;
        size_t needed;
        size_t done;

        write_buffer() = default; // TODO throw logic error
        write_buffer(const char*, size_t);
    };

    struct connect_buffer {
        const char* ip;
        int port;

        connect_buffer() = default;
        connect_buffer(const char*, int);
    };

    struct io_service {

        io_service();
        io_service(const io_service&) = delete;
        io_service(io_service&&) = default;

        void run();
        void stop();

        void read_waiter(int, char*, size_t, std::function <void(const char*)>);
        void write_waiter(int, const char*, size_t, std::function <void()>);
        void accept_waiter(int, std::function <void(int)>);
        void connect_waiter(int, const char*, int, std::function <void()>);

        ~io_service();
    private:
        epoll efd;
        int stopper;

        std::map <int, std::function<void(int)> > accept_callback;

        std::map <int, std::function<void()> > connect_callback;
        std::map <int, connect_buffer> connect_buf;

        std::map <int, read_buffer> read_buf;
        std::map <int, std::function<void(const char*)> > read_callback;

        std::map <int, write_buffer> write_buf;
        std::map <int, std::function<void()> > write_callback;
    };
}

#endif // IO_SERVICE