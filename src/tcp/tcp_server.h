#ifndef TCP_SERVER
#define TCP_SERVER

#include "tcp_socket.h"

namespace tcp {

    struct tcp_server {
        tcp_server();
        tcp_server(const tcp_server &) = delete;
        tcp_server(tcp_server &&) = default;
        void bind(char const*, int);
        void listen();
        int get_fd();
        tcp_socket * get_connection();
        ~tcp_server();

    private:
        int fd;
    };
}

#endif //TCP_SERVER