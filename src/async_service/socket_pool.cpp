#include <unistd.h>
#include <stdexcept>
#include <string.h>
#include "socket_pool.h"
using namespace std;

std::set <int> socket_pool::set_fd;

void socket_pool::add(int i) {
    if ((set_fd).count(i) == 0)
        set_fd.insert(i);
}

void socket_pool::remove(int i) {
    if (set_fd.count(i) > 0) {
        if (::close(i) < 0)
            std::runtime_error(strerror(errno));
        set_fd.erase(i);
    }
}

socket_pool::~socket_pool() {
    set<int>::iterator it = set_fd.begin();
    for (; it != set_fd.end(); it++)
        if (::close((*it)) < 0)
            std::runtime_error(strerror(errno));
}
