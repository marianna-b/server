#include <tcp/async_server.h>
#include <iostream>

using namespace tcp;
using namespace std;

int main() {
    string ip = "127.0.0.2";
    int port = 33344;

    async_socket client;
    io_service service;
    char input[256];
    size_t input_size;

    function<void(async_type<async_socket>)> on_stop = [&](async_type<async_socket> s) {
        service.stop();
    };                signalfd_siginfo siginfo;

    function<void(async_type<async_socket>)> on_input = [&](async_type<async_socket> client2){
        cin >> input;
        input_size = string(input).size() + 1;

        function<void(async_type<async_socket>)> on_send = [&](async_type<async_socket> client3){
            if (string(input) == "stop")
                client3.get().write(&service, input, input_size, on_stop);
            else
                client3.get().write(&service, input, input_size, on_input);
        };

        uint32_t length = (uint32_t) input_size;
        client2.get().write(&service, &length, 4, on_send);
    };

    client.set_connection(&service, ip.c_str(), port, on_input);
    if (!service.run()) {
        cerr << "SIG ARRIVED!\n";
        client.close();
        return 0;
    }
    client.close();
    return 0;
}
