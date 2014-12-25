#include "io_service.h"
#include "signal_handler.h"
#include <sys/eventfd.h>
#include <iostream>
#include <string.h>
#include <tcp/async_server.h>

using namespace tcp;
using namespace std;

void reader_del_epoll(epoll*, int, io_events*);
void writer_del_epoll(epoll*, int, io_events*);

tcp::io_service::io_service() {
    efd = new epoll();
    clean = false;

    stopper = ::eventfd(0, 0);
    efd->add(stopper, EPOLL_EVENTFD);
    signal_handler::add(stopper);

    pause_fd = ::eventfd(0, 0);
    efd->add(pause_fd, EPOLL_EVENTFD);

    if (stopper < 0)
        throw runtime_error(strerror(errno));
}

void tcp::io_service::run() {
    bool running = true;
    while (running) {
        int n = efd->wait();
        if (n < 0) {
            if (errno != EINTR) {
                throw runtime_error(strerror(errno));
            } else {
                break;
            }
        }
        if (n == 0) continue;

        for (int i = 0; i < n; ++i) {
            int curr = efd->events[i].data.fd;

            if (curr == stopper) {
                cerr << "Trying to stop service!\n";
                cerr << "Success!\n";
                running = false;
                break;
            }

            if (curr == pause_fd) {
                cerr << "Trying to stop service clean!\n";
                cerr << "Success!\n";
                clean = true;
                running = false;
                break;
            }

            io_events *ev = &data[curr];

            if (efd->events[i].events & EPOLL_WRITE) {
                if (ev->want_connect() && ev->run_connect()) {
                    cerr << "Call of callback!\n";
                    writer_del_epoll(efd, curr, ev);
                    ev->connect_call_back();
                }

                if (ev->want_write() && ev->run_write()) {
                    cerr << "Call of callback!\n";
                    writer_del_epoll(efd, curr, ev);
                    ev->write_call_back();
                }
            }

            if (efd->events[i].events & EPOLL_READ) {
                if (ev->want_accept() && ev->run_accept()) {
                    cerr << "Call of callback!\n";
                    reader_del_epoll(efd, curr, ev);
                    ev->accept_call_back();
                }

                if (ev->want_read() && ev->run_read()) {
                    cerr << "Call of callback!\n";
                    reader_del_epoll(efd, curr, ev);
                    ev->read_call_back();
                }
            }
        }
    }
    if (clean) {
        delete efd;
        efd = new epoll();
        clean = false;
        data.clear();
    }
}

void tcp::io_service::stop() {
    cerr << "SERVICE STOP REQUEST SENT!\n";
    if (eventfd_write(stopper, 1) < 0)
        throw runtime_error(strerror(errno));
}

void io_service::pause() {
    cerr << "SERVICE PAUSE REQUEST SENT!\n";
    if (eventfd_write(pause_fd, 1) < 0)
        throw runtime_error(strerror(errno));
}

tcp::io_service::~io_service() {
    delete efd;
    if(::close(stopper) < 0)
        throw runtime_error(strerror(errno));

    if(::close(pause_fd) < 0)
        throw runtime_error(strerror(errno));

}

void reader_del_epoll(epoll* efd, int fd, io_events* ev) {
    size_t w = ev->get_writers();
    size_t r = ev->get_readers();

    if (r == 1) {
        if (w == 0) {
            efd->remove(fd);
        } else {
            efd->modify(fd, EPOLL_WRITE);
        }
    }
}

void writer_del_epoll(epoll* efd, int fd, io_events* ev) {

    size_t w = ev->get_writers();
    size_t r = ev->get_readers();

    if (w == 1) {
        if (r == 0) {
            efd->remove(fd);
        } else {
            efd->modify(fd, EPOLL_READ);
        }
    }
}

void reader_add_epoll(epoll* efd, int fd, io_events* ev) {
    size_t w = ev->get_writers();
    size_t r = ev->get_readers();
    if (r == 0) {
        if (w == 0) {
            efd->add(fd, EPOLL_READ);
        } else {
            efd->modify(fd, EPOLL_BOTH);
        }
    }
}

void writer_add_epoll(epoll* efd, int fd, io_events* ev) {
    size_t w = ev->get_writers();
    size_t r = ev->get_readers();

    if (w == 0) {
        if (r == 0) {
            efd->add(fd, EPOLL_WRITE);
        } else {
            efd->modify(fd, EPOLL_BOTH);
        }
    }
}

void io_service::read_waiter(async_socket* s, size_t size, function<void(int, async_socket*, size_t, void*)> f) {
    int fd = s->get_fd();

    if (data.count(fd) == 0)
        data[fd] = io_events(s);

    reader_add_epoll(efd, fd, &data[fd]);
    data[fd].add_read(read_buffer(true, size, f));
}

void io_service::read_some_waiter(async_socket* s, size_t size, function<void(int, async_socket*, size_t, void*)> f) {
    int fd = s->get_fd();

    if (data.count(fd) == 0)
        data[fd] = io_events(s);

    reader_add_epoll(efd, fd, &data[fd]);
    data[fd].add_read(read_buffer(false, size, f));
}

void io_service::write_waiter(async_socket* s, void * mesg, size_t size, function<void(int, async_socket*)> f) {
    int fd = s->get_fd();

    if (data.count(fd) == 0)
        data[fd] = io_events(s);

    writer_add_epoll(efd, fd, &data[fd]);
    data[fd].add_write(write_buffer(mesg, size, f));
}

void io_service::accept_waiter(async_server* s, function<void(int, async_socket*)> f) {
    int fd = s ->get_fd();

    if (data.count(fd) == 0)
        data[fd] = io_events(fd);

    reader_add_epoll(efd, fd, &data[fd]);
    data[fd].add_accept(accept_buffer(f));
}

void io_service::connect_waiter(async_socket* s, const char* ip, int port, function<void(int, async_socket*)> f) {
    int fd = s->get_fd();

    if (data.count(fd) == 0)
        data[fd] = io_events(s);

    writer_add_epoll(efd, fd, &data[fd]);
    data[fd].add_connect(connect_buffer(ip, port, f));
}

bool io_service::operator<(io_service const &aConst) const {
    return stopper < aConst.stopper;
}

void io_service::del_client(async_server *server, async_socket* sock) {
    io_events& events = data[server->get_fd()];
    events.remove_client(sock);
}
