#ifndef TCP_SERVER
#define TCP_SERVER

#include "socket.h"
#include "client.h"

namespace tcp {

    struct server : socket {
        server();
        server(const server&) = delete;
        server(server&&) = default;
        void bind(char const*, int);
        client* get_connection();
    };
}

#endif //TCP_SERVER