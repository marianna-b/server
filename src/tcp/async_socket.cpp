#include <string.h>
#include <iostream>
#include "async_socket.h"
using namespace std;
using namespace tcp;


async_socket::async_socket()
        : fd(::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) {

    if (fd == -1) {
        throw runtime_error(strerror(errno));
    }
}

async_socket::async_socket(int cfd) {
    fd = cfd;
}

void async_socket::set_connection(io_service *service, char const *ip, int port, function<void()> f) {
    service->connect_waiter(fd, ip, port, f);
}

void async_socket::read(io_service *service, size_t t, function<void(const char*)> f) {
    service->read_waiter(fd, t, f);
}

void async_socket::write(io_service *service, char const *msg, size_t t, function<void()> f) {
    service->write_waiter(fd, msg, t, f);
}

async_socket::~async_socket() {
    //close(fd);
}

int async_socket::get_fd() {
    return fd;
}

