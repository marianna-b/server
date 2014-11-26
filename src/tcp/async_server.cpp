#include <string.h>
#include "async_server.h"

using namespace std;
using namespace tcp;


async_server::async_server(): fd(::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) {
    if (fd == -1) {
        throw runtime_error(strerror(errno));
    }
}

void async_server::bind(char const *ip, int port) {
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ::inet_addr(ip);
    addr.sin_port = ::htons((uint16_t) port);

    int flag = ::bind(fd, (struct sockaddr *) &addr, sizeof(struct sockaddr_in));

    if (flag == -1) {
        throw std::runtime_error(strerror(errno));
    }
}

void async_server::listen() {
    if(::listen(fd, 20) < 0) {
        throw std::runtime_error(strerror(errno));
    }
}

int async_server::get_fd() {
    return fd;
}

void async_server::get_connection(io_service* service, function<void(async_type<async_socket>)> callback) {
    service -> accept_waiter(fd, callback);
}

async_server::~async_server() {
    //close(fd);
}

void async_server::close() {
    ::close(fd);
}
