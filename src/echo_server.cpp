#include <tcp/async_server.h>
#include <iostream>

using namespace tcp;
using namespace std;

int main() {
    string ip = "127.0.0.2";
    int port = 33344;

    async_server server;
    io_service service;
    server.bind(ip.c_str(), port);
    server.listen();

    function<void(int, void*)> on_read;

    auto on_get_msg = [&](int fd, void* s){
        async_socket client(fd);
        cout << (char *) s << endl;

        if (string((char*)s) == "stop")
            service.stop();
        else {
            client.read(&service, 4, on_read);
        }
    };

    on_read = [&](int fd, void* ptr) {
        async_socket client(fd);
        uint32_t length = *((uint32_t*) ptr);
        client.read(&service, length, on_get_msg);
    };

    auto on_accept = [&](int fd){
        async_socket client2(fd);
        client2.read(&service, 4, on_read);
    };

    server.get_connection(&service, on_accept);
    service.run();
    server.close();
    return 0;
}
