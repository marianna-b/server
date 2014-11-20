#include "socket.h"

#include <stdexcept>
#include <cstring>

using namespace tcp;
using namespace std;

socket::socket()
    : fd(::socket(AF_INET, SOCK_STREAM, 0)) {

    if (fd == -1) {
        throw runtime_error(strerror(errno));
    }
}

size_t socket::read(char *pVoid, size_t t) {
    return (size_t) ::read(fd, pVoid, t);
}

size_t socket::write(const char *pVoid, size_t t) {
    return (size_t) ::write(fd, pVoid, t);
}

socket::~socket() {
    close(fd);
}

