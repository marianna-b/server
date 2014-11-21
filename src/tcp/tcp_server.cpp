#include <string.h>
#include <stdexcept>
#include "tcp_server.h"

using namespace tcp;
using namespace std;

void tcp_server::bind(char const *ip, int port) {
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ::inet_addr(ip);
    addr.sin_port = ::htons((uint16_t) port);

    int flag = ::bind(fd, (struct sockaddr *) &addr, sizeof(struct sockaddr_in));

    if (flag == -1) {
        throw std::runtime_error(strerror(errno));
    }
}

tcp::tcp_socket *tcp::tcp_server::get_connection() {
    if(listen(fd, 20) < 0) {
        throw std::runtime_error(strerror(errno));
    }
    sockaddr_in my_addr;
    socklen_t addr_size = sizeof(sockaddr_in);
    int cfd = ::accept(fd, (struct sockaddr *) &my_addr, &addr_size);
    return new tcp_socket(cfd);
}

tcp_server::tcp_server() : fd(::socket(AF_INET, SOCK_STREAM, 0)) {
    if (fd == -1) {
        throw runtime_error(strerror(errno));
    }
}

tcp_server::~tcp_server() {
    close(fd);
}
