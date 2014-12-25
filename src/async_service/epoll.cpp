#include "epoll.h"
#include <unistd.h>
#include <stdexcept>
#include <string.h>
#include <iostream>

using namespace std;

using namespace tcp;

epoll::epoll() {
    fd = ::epoll_create(1);
    if (fd < 0)
        throw std::runtime_error(strerror(errno));
}

void epoll::add(int f, epoll_type type) {
    epoll_event ev;
    ev.data.ptr = nullptr;
    ev.data.fd = f;
    ev.events = type;
    if (::epoll_ctl(fd, EPOLL_CTL_ADD, f, &ev) < 0)
        throw logic_error(strerror(errno));
}


void epoll::modify(int f, epoll_type type) {
    epoll_event ev;
    ev.data.fd = f;
    ev.events = type;
    if (::epoll_ctl(fd, EPOLL_CTL_MOD, f, &ev) < 0)
        throw logic_error(strerror(errno));
}

void epoll::remove(int f) {
    if (::epoll_ctl(fd, EPOLL_CTL_DEL, f, NULL) < 0)
        throw logic_error(strerror(errno));}

int epoll::wait() {
    return ::epoll_wait(fd, events, MAX_EVENTS, timeout);
}

epoll::~epoll() {
    ::close(fd);
}

