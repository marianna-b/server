#include <string.h>
#include <sys/wait.h>
#include <iostream>
#include "signal_handler.h"
#include "io_service.h"

using namespace tcp;

volatile int signal_handler::size = 0;
volatile io_service* signal_handler::services[MAX_SERVICES];

void tcp::signal_handler::add(io_service* i) {
    services[size] = i;
    size++;
}

void tcp::signal_handler::run_handler(int i) {
    std::cerr << "Signal caught " << i << " \n";
    for (int i = 0; i < size; ++i) {
        services[i]->efd->add(services[i]->stopper, EPOLL_READ);
        uint32_t a = 1;
        ::write(services[i]->stopper, &a, 8);
    }
    exit(i);
}

void tcp::signal_handler::set() {
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_handler = signal_handler::run_handler;
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGTERM);
    act.sa_mask = set;
    sigaction(SIGINT, &act, 0);
    sigaction(SIGTERM, &act, 0);

}
