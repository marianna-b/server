#include "epoll.h"
#include <unistd.h>
#include <stdexcept>
#include <string.h>

using namespace tcp;

epoll::epoll() {
    fd = ::epoll_create(1);
}

void epoll::add(int f, epoll_type type) {
    epoll_event ev;
    ev.data.fd = f;
    ev.events = type;
    if (::epoll_ctl(fd, EPOLL_CTL_ADD, f, &ev) < 0)
        throw std::runtime_error(strerror(errno));
}

void epoll::remove(int f) {
    if (::epoll_ctl(fd, EPOLL_CTL_DEL, f, NULL) < 0)
        throw std::runtime_error(strerror(errno));}

int epoll::wait() {
    return ::epoll_wait(fd, events, max_events, timeout);
}

epoll::~epoll() {
    ::close(fd);
}
