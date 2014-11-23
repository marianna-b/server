#include "io_service.h"
#include <sys/eventfd.h>
#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace tcp;
using namespace std;

tcp::io_service::io_service() :efd() {
    stopper = ::eventfd(10, EFD_NONBLOCK);
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
                running = false;
                break;
            }

            epoll_type type = fd_type[curr];
            if (type == EPOLL_WRITE) {
                cerr << "Trying to write! " << curr <<"\n";

                const char* buffer = write_buf[curr].buf;
                size_t idx = write_buf[curr].done;
                size_t idx2 = write_buf[curr].needed;
                ssize_t w = ::send(curr, buffer + idx, idx2 - idx, MSG_DONTWAIT);
                if (w < 0) {
                    if (errno != EAGAIN && errno != EWOULDBLOCK) {
                        throw std::runtime_error(strerror(errno));
                    }
                    cerr << "We need some time!\n";
                } else {
                    cerr << "Success!\n";
                    cerr << "We've written " << w << "!\n";
                    write_buf[curr].done += w;
                    if (write_buf[curr].done == write_buf[curr].needed) {
                        cerr << "Call of callback!\n";
                        efd.remove(curr);
                        write_callback[curr](curr);
                    }
                }
            }

            if (type == EPOLL_READ) {
                cerr << "Trying to read! " << curr <<"\n";
                char buffer[256];
                size_t idx = read_buf[curr].done;
                size_t idx2 = read_buf[curr].needed;
                ssize_t r = ::recv(curr, buffer, idx2 - idx, MSG_DONTWAIT);
                read_buf[curr].buf += string(buffer);

                if (r < 0) {
                    if (errno != EAGAIN && errno != EWOULDBLOCK) {
                        throw std::runtime_error(strerror(errno));
                    }
                    cerr << "We need some time!\n";
                } else {
                    cerr << "Success!\n";
                    cerr << "We've read " << r << "!\n";
                    read_buf[curr].done += r;

                    if (read_buf[curr].done == read_buf[curr].needed) {
                        cerr << "Call of callback!\n";
                        efd.remove(curr);
                        read_callback[curr](curr, read_buf[curr].buf);
                    }
                }
            }

            if (type == EPOLL_ACCEPT) {
                cerr << "Trying to accept! " << curr <<"\n";
                sockaddr_in addr;
                socklen_t addr_size;
                int flag = ::accept4(curr, (sockaddr *) &addr, &addr_size, SOCK_NONBLOCK);

                if (flag < 0) {
                    if (errno != EAGAIN && errno != EWOULDBLOCK) {
                        throw std::runtime_error(strerror(errno));
                    }
                    cerr << "We need some time!\n";
                } else {
                    cerr << "Success! Client " << flag << "\n";
                    cerr << "Call of callback!\n";
                    efd.remove(curr);
                    accept_callback[curr](flag);
                }
            }

            if (type == EPOLL_CONNECT) {
                cerr << "Trying to connect! " << curr <<"\n";
                sockaddr_in addr;
                const char* ip = connect_buf[curr].ip;
                int port = connect_buf[curr].port;

                addr.sin_family = AF_INET;
                addr.sin_addr.s_addr = ::inet_addr(ip);
                addr.sin_port = ::htons((uint16_t) port);

                int flag = ::connect(curr, (struct sockaddr *)&addr, sizeof(addr));
                if (flag < 0) {
                    if (errno != EALREADY && errno != EINPROGRESS) {
                        throw std::runtime_error(strerror(errno));
                    }
                    cerr << "We need some time!\n";
                } else {
                    cerr << "Success!\n";
                    cerr << "Call of callback!\n";
                    efd.remove(curr);
                    connect_callback[curr](curr);
                }
            }
        }
    }
}

void tcp::io_service::stop() {
    cerr << "SERVICE STOPPING!\n";
    efd.add(stopper, EPOLL_READ);
    ::write(stopper, "close", 6);
}

tcp::io_service::~io_service() {
    ::close(stopper);
}

void io_service::read_waiter(int fd, size_t size, function<void(int, string)> f) {
    efd.add(fd, EPOLL_READ);
    fd_type[fd] = EPOLL_READ;
    read_buf[fd] = read_buffer(size);
    read_callback[fd] = f;
}

void io_service::write_waiter(int fd, char const* mesg, size_t size, function<void(int)> f) {
    efd.add(fd, EPOLL_WRITE);
    fd_type[fd] = EPOLL_WRITE;
    write_buf[fd] = write_buffer(mesg, size);
    write_callback[fd] = f;
}


void io_service::accept_waiter(int fd, function<void(int)> f) {
    efd.add(fd, EPOLL_READ);
    fd_type[fd] = EPOLL_ACCEPT;
    accept_callback[fd] = f;
}

read_buffer::read_buffer(size_t n) {
    buf = "";
    needed = n;
    done = 0;
}

write_buffer::write_buffer(char const *b, size_t n) {
    buf = b;
    needed = n;
    done = 0;
}

void io_service::connect_waiter(int fd, const char* ip, int port, function<void(int)> f) {
    efd.add(fd, EPOLL_WRITE);
    fd_type[fd] = EPOLL_CONNECT;
    connect_callback[fd] = f;
    connect_buf[fd] = connect_buffer(ip, port);
}

connect_buffer::connect_buffer(char const *i, int p) {
   ip = i;
   port = p;
}

