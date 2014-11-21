#include "tcp_socket.h"

#include <stdexcept>
#include <cstring>

using namespace tcp;
using namespace std;

tcp_socket::tcp_socket()
    : fd(::socket(AF_INET, SOCK_STREAM, 0)) {

    if (fd == -1) {
        throw runtime_error(strerror(errno));
    }
}

size_t tcp_socket::read(char *pVoid, size_t t) {
    return (size_t) ::read(fd, pVoid, t);
}

size_t tcp_socket::write(const char *pVoid, size_t t) {
    return (size_t) ::write(fd, pVoid, t);
}

tcp_socket::~tcp_socket() {
    close(fd);
}

void tcp_socket::set_connection(char const* ip, int port) {
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ::inet_addr(ip);
    addr.sin_port = ::htons((uint16_t) port);
    if (::connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        throw std::runtime_error(strerror(errno));
    }
}

tcp_socket::tcp_socket(int i){
    fd = i;
}