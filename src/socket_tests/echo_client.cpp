#include <tcp/async_server.h>
#include <iostream>

using namespace tcp;
using namespace std;

int main() {
    string ip = "127.0.0.2";
    int port = 33345;

    async_socket* client = new async_socket();
    io_service service;
    char input[256];
    size_t input_size;

    function<void(int, async_socket*)> on_stop = [&](int, async_socket* s) {
        service.stop();
    };

    function<void(int, async_socket*)> on_input = [&](int, async_socket* client2){
        cin >> input;
        input_size = string(input).size() + 1;

        function<void(int, async_socket*)> on_send = [&](int, async_socket* client3){
            if (string(input) == "stop")
                client3->write(&service, input, input_size, on_stop);
            else
                client3->write(&service, input, input_size, on_input);
        };

        uint32_t length = (uint32_t) input_size;
        client2->write(&service, &length, 4, on_send);
    };

    client->set_connection(&service, ip.c_str(), port, on_input);
    service.run();
    delete(client);
    return 0;
}
