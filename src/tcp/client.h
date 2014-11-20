#ifndef TCP_CLIENT
#define TCP_CLIENT

#include "socket.h"

namespace tcp {

    struct client : socket {
        client();
        client(int);
        client(const client&) = delete;
        client(client&&) = default;
        void set_connection(char const* ip, int port);
    };
}

#endif //TCP_CLIENT