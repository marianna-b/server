#include <string.h>
#include <iostream>
#include <async_service/io_service.h>
#include <printf.h>
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

void async_socket::set_connection(io_service *service, char const *ip, int port, function<void(int, async_socket*)> f) {
    services.insert(service);
    service->connect_waiter(this, ip, port, f);
}

void async_socket::read(io_service *service, size_t t, function<void(int, async_socket*, void*)> f) {
    services.insert(service);
    service->read_waiter(this, t, f);
}

void async_socket::write(io_service *service, void *msg, size_t t, function<void(int, async_socket*)> f) {
    services.insert(service);
    service->write_waiter(this, msg, t, f);
}

async_socket::~async_socket() {
    std::set<io_service*>::iterator it = services.begin();
    for (; it != services.end(); ++it) {
        io_service* service = *it;
        service->data.erase(fd);
    }
    ::close(fd);
}

int async_socket::get_fd() {
    return fd;
}

void async_socket::read_some(io_service *service, size_t t, function<void(int, async_socket*, void*)> f) {
    services.insert(service);
    service->read_some_waiter(this, t, f);
}
