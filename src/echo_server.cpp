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

    function<void(async_socket, void*)> on_read;

    function<void(async_socket, void*)> on_get_msg = [&](async_socket client, void* s){
        cout << (char *) s << endl;

        if (string((char*)s) == "stop")
            service.stop();
        else {
            client.read(&service, 4, on_read);
        }
    };

    on_read = [&](async_socket client, void* ptr) {
        uint32_t length = *((uint32_t*) ptr);
        client.read(&service, length, on_get_msg);
    };

    function<void(async_socket)>on_accept = [&](async_socket client2){
        client2.read(&service, 4, on_read);
    };

    server.get_connection(&service, on_accept);
    service.run();
    server.close();
    return 0;
}
