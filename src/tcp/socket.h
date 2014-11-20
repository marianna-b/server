#ifndef TCP_SOCK_FD
#define TCP_SOCK_FD

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

namespace tcp {
    struct socket {
        socket();
        socket(const socket&) = delete;
        socket(socket&&) = default;

        size_t read(char*, size_t);

        size_t write(const char*, size_t);

        ~socket();

    protected:
        socket(int fd) : fd(fd) {}

        int fd;
    };
}

#endif // TCP_SOCK_FD