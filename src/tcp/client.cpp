#include <stdexcept>
#include <string.h>
#include "client.h"
using namespace tcp;
using namespace std;

void client::set_connection(char const* ip, int port) {
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ::inet_addr(ip);
    addr.sin_port = ::htons((uint16_t) port);
    if (::connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        throw std::runtime_error(strerror(errno));
    }
}

client::client(int i) {
    fd = i;
}

client::client() : socket(::socket(AF_INET, SOCK_STREAM, 0)) {
    if (fd == -1) {
        throw runtime_error(strerror(errno));
    }
}
