#ifndef ASYNC_SOCK
#define ASYNC_SOCK

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <async_service/io_service.h>

namespace tcp {
    struct async_socket {
        async_socket();
        async_socket(int);
        async_socket(const async_socket &) = delete;
        async_socket(async_socket &&) = default;
        void set_connection(io_service*, char const* ip, int port, std::function<void()>);
        void read(io_service*, size_t, std::function<void(const char*)>);
        void write(io_service*, const char*, size_t, std::function<void()>);
        ~async_socket();

        int get_fd();

    protected:
        int fd;
    };
}

#endif // ASYNC_SOCK