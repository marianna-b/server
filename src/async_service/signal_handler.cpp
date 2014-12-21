#include <string.h>
#include <sys/wait.h>
#include <iostream>
#include <sys/eventfd.h>
#include "signal_handler.h"
#include "io_service.h"

using namespace tcp;

volatile int signal_handler::size = 0;
volatile int signal_handler::services[MAX_SERVICES];

void tcp::signal_handler::add(int i) {
    services[size] = i;
    size++;
}

void tcp::signal_handler::run_handler(int i) {
    for (int i = 0; i < size; ++i) {
        eventfd_write(services[i], 1);
    }
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
