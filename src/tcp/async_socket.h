#ifndef ASYNC_SOCK
#define ASYNC_SOCK

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <functional>

namespace tcp {
    struct async_socket {
        async_socket();
        async_socket(int);
        async_socket(const async_socket &) = default;
        async_socket(async_socket &&) = default;
        void set_connection(struct io_service*, char const* ip, int port, std::function<void(async_socket)>);
        void read(struct io_service*, size_t, std::function<void(async_socket, void*)>);
        void write(struct io_service*, void*, size_t, std::function<void(async_socket)>);
        ~async_socket();

        int get_fd();
        void close();

    protected:
        int fd;
    };
}

#endif // ASYNC_SOCK