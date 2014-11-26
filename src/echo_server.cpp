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

    function<void(async_type<async_socket>, async_type<void*>)> on_read;

    function<void(async_type<async_socket>, async_type<void*>)> on_get_msg =
            [&](async_type<async_socket> client, async_type<void*> s){
        cout << (char *) s.get() << endl;

        if (string((char*)s.get()) == "stop")
            service.stop();
        else {
            client.get().read(&service, 4, on_read);
        }
    };

    on_read = [&](async_type<async_socket> client, async_type<void*> ptr) {
        uint32_t length = *((uint32_t*) ptr.get());
        client.get().read(&service, length, on_get_msg);
    };

    function<void(async_type<async_socket>)>on_accept = [&](async_type<async_socket> client2){
        client2.get().read(&service, 4, on_read);
    };

    server.get_connection(&service, on_accept);
    service.run();
    server.close();
    return 0;
}
