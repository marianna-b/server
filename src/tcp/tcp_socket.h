#ifndef TCP_SOCK_FD
#define TCP_SOCK_FD

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

namespace tcp {
    struct tcp_socket {
        tcp_socket();
        tcp_socket(int);
        tcp_socket(const tcp_socket &) = delete;
        tcp_socket(tcp_socket &&) = default;
        void set_connection(char const* ip, int port);
        size_t read(char*, size_t);
        size_t write(const char*, size_t);
        ~tcp_socket();
    protected:
        int fd;
    };
}

#endif // TCP_SOCK_FD