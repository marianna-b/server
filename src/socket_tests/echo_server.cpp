#include <tcp/async_server.h>
#include <iostream>

using namespace tcp;
using namespace std;

int main() {
    string ip = "127.0.0.2";
    int port = 33345;

    async_server* server = new async_server();
    async_socket* client = 0;
    async_socket* client2 = 0;
    io_service service;
    server->bind(ip.c_str(), port);
    server->listen();

    function<void(int, async_socket*, void*)> on_read;

    function<void(int, async_socket*, void*)> on_get_msg =
            [&](int, async_socket* client, void* s){
        cout << (char *) s << endl;

        if (string((char*)s) == "stop")
            service.stop();
        else {
            client->read(&service, 4, on_read);
        }
    };

    on_read = [&](int, async_socket* client, void* ptr) {
        uint32_t length = *((uint32_t*) ptr);
        client->read(&service, length, on_get_msg);
    };

    function<void(int, async_socket*)>on_accept = [&](int, async_socket* c){
        if (client != 0)
            client2 = c;
        else
            client = c;
        c->read(&service, 4, on_read);
    };

    server->get_connection(&service, on_accept);
    server->get_connection(&service, on_accept);
    service.run();

    delete(server);
    delete(client);
    delete(client2);
    return 0;
}
