#ifndef ASYNC_SERVER
#define ASYNC_SERVER

#include <functional>
#include <async_service/io_service.h>
#include "async_socket.h"

namespace tcp {

    struct async_server {
        async_server();
        async_server(const async_server &) = delete;
        async_server(async_server &&) = default;
        void bind(char const*, int);
        void listen();
        int get_fd();
        void get_connection(io_service*, std::function<void(int)>);
        ~async_server();

    private:
        int fd;
    };
}

#endif //ASYNC_SERVER