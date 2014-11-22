#include <thread>
#include <iostream>
#include <tcp/tcp_socket.h>
#include <async_service/io_service.h>

using namespace std;
using namespace tcp;

int main()
{
    tcp::io_service s;
    std::thread t([&]() {
        s.run();
    });
    s.stop();
    t.join();
    return 0;
}