#include "io_service.h"
#include <sys/eventfd.h>
#include <iostream>
#include <string.h>

using namespace tcp;
using namespace std;

void reader_del_epoll(epoll*, int, io_events*);
void writer_del_epoll(epoll*, int, io_events*);

tcp::io_service::io_service() :efd() {
    clean = false;
    stopper = ::eventfd(10, EFD_NONBLOCK);
    if (stopper < 0)
        throw runtime_error(strerror(errno));
}

void tcp::io_service::run() {
    bool running = true;
    while (running) {
        int n = efd.wait();
        if (n < 0) throw runtime_error(strerror(errno));
        if (n == 0) continue;

        for (int i = 0; i < n; ++i) {
            int curr = efd.events[i].data.fd;

            if (curr == stopper) {
                cerr << "Trying to stop service!\n";
                cerr << "Success!\n";
                char c[256];
                if (::read(stopper, c, sizeof(uint64_t)) < 0)
                    throw runtime_error(strerror(errno));

                uint64_t f = *((uint64_t*) c);
                if (f == 2)
                    clean = true;
                running = false;
                break;
            }

            io_events *ev = &data[curr];

            if (efd.events[i].events & EPOLL_WRITE) {
                if (ev->want_connect() && ev->run_connect()) {
                    cerr << "Call of callback!\n";
                    ev->connect_call_back();
                    writer_del_epoll(&efd, curr, ev);
                }

                if (ev->want_write() && ev->run_write()) {
                    cerr << "Call of callback!\n";
                    ev->write_call_back();
                    writer_del_epoll(&efd, curr, ev);
                }
            }

            if (efd.events[i].events & EPOLL_READ) {
                if (ev->want_accept() && ev->run_accept()) {
                    cerr << "Call of callback!\n";
                    ev->accept_call_back();
                    reader_del_epoll(&efd, curr, ev);
                }

                if (ev->want_read() && ev->run_read()) {
                    cerr << "Call of callback!\n";
                    ev->read_call_back();
                    reader_del_epoll(&efd, curr, ev);
                }
            }
        }
    }
    if (clean) {
        efd = epoll();
        clean = false;
        data.empty();
    }
}

void tcp::io_service::stop() {
    cerr << "SERVICE STOP REQUEST SENT!\n";
    efd.add(stopper, EPOLL_READ);
    uint64_t a = 1;
    if (::write(stopper, &a, sizeof(uint64_t)) < 0)
        throw runtime_error(strerror(errno));
}

void io_service::clean_stop() {
    cerr << "SERVICE STOP&CLEAN REQUEST SENT!\n";
    uint64_t a = 2;
    efd.add(stopper, EPOLL_READ);
    if (::write(stopper, &a, sizeof(uint64_t)) < 0)
        throw runtime_error(strerror(errno));
}

tcp::io_service::~io_service() {
    if(::close(stopper) < 0)
        throw runtime_error(strerror(errno));
    efd.close();
}

void reader_del_epoll(epoll* efd, int fd, io_events* ev) {

    size_t w = ev->get_writers();
    size_t r = ev->get_readers();

    if (r == 0)
        if (w == 0) {
            efd->remove(fd);
        } else {
            efd->modify(fd, EPOLL_WRITE);
        }
}

void writer_del_epoll(epoll* efd, int fd, io_events* ev) {

    size_t w = ev->get_writers();
    size_t r = ev->get_readers();

    if (w == 0)
        if (r == 0) {
            efd->remove(fd);
        } else {
            efd->modify(fd, EPOLL_READ);
        }
}

void reader_add_epoll(epoll* efd, int fd, io_events* ev) {

    size_t w = ev->get_writers();
    size_t r = ev->get_readers();
    if (r == 0)
        if (w == 0) {
            efd->add(fd, EPOLL_READ);
        } else {
            efd->modify(fd, EPOLL_BOTH);
        }
}

void writer_add_epoll(epoll* efd, int fd, io_events* ev) {

    size_t w = ev->get_writers();
    size_t r = ev->get_readers();

    if (w == 0)
        if (r == 0) {
            efd->add(fd, EPOLL_WRITE);
        } else {
            efd->modify(fd, EPOLL_BOTH);
        }
}

void io_service::read_waiter(int fd, size_t size, function<void(async_type<async_socket>, async_type<void*>)> f) {
    if (data.count(fd) == 0)
        data[fd] = io_events(fd);
    reader_add_epoll(&efd, fd, &data[fd]);
    data[fd].add_read(read_buffer(size, f));
}

void io_service::write_waiter(int fd, void * mesg, size_t size, function<void(async_type<async_socket>)> f) {
    if (data.count(fd) == 0)
        data[fd] = io_events(fd);
    writer_add_epoll(&efd, fd, &data[fd]);
    data[fd].add_write(write_buffer(mesg, size, f));
}

void io_service::accept_waiter(int fd, function<void(async_type<async_socket>)> f) {
    if (data.count(fd) == 0)
        data[fd] = io_events(fd);

    reader_add_epoll(&efd, fd, &data[fd]);
    data[fd].add_accept(accept_buffer(f));
}

void io_service::connect_waiter(int fd, const char* ip, int port, function<void(async_type<async_socket>)> f) {
    if (data.count(fd) == 0)
        data[fd] = io_events(fd);

    writer_add_epoll(&efd, fd, &data[fd]);
    data[fd].add_connect(connect_buffer(ip, port, f));
}

