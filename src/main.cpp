#include <thread>
#include <iostream>
#include <async_service/io_service.h>
#include <tcp/async_server.h>

using namespace std;
using namespace tcp;

int main()
{
    string ip = "127.0.0.1";
    int port = 33334;
    string message1 = "This is message #1";
    string message2 = "This is message #2 ";

    async_server s;
    io_service service;
    s.bind(ip.c_str(), port);
    s.listen();

    size_t need1 = message1.length() + 1;
    size_t need2 = message2.length() + 1;
    async_socket c;
    function<void()> cliFunc1 = [&]() {
        cerr << "Connection established " << c.get_fd() << "!\n";

        function<void()> cliFunc2 = [&](){
            cerr << "Message #1 written!\n";

            /*function<void(const char*)> cliFunc3 = [&](const char* buf){
                cerr << "Message #2 read:  " << buf << "\n";
            };
            c.read(&service, need2, cliFunc3);*/
        };
        c.write(&service, message1.c_str(), need1, cliFunc2);
    };
    c.set_connection(&service, ip.c_str(), port, cliFunc1);

    function<void(int)> servFunc1 = [&](int fd) {
        async_socket client(fd);
        cerr << "Client accepted " << fd << "!\n";

        function <void(const char*)> servFunc2 = [&](const char* buf) {
            cerr << "Message #1 read: " << buf << "\n";
            service.stop();
/*
            function <void()> servFunc3 = [&](){ cerr << "Message #2 written!\n"; };

            client.write(&service, message2.c_str(), need2, servFunc3);
 */       };

        cerr << "Preparing to read " << client.get_fd() << "\n";
        client.read(&service, need1, servFunc2);
    };

    s.get_connection(&service, servFunc1);
    service.run();
    return 0;
}
