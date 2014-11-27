#include <set>

#ifndef SOCKET_POOL
#define SOCKET_POOL

struct socket_pool {

    static void add(int);
    static void remove(int);

    ~socket_pool();

private:
    static std::set <int> set_fd;
};



#endif