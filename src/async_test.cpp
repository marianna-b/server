#include <thread>
#include <iostream>
#include <async_service/io_service.h>
#include <tcp/async_server.h>

using namespace std;
using namespace tcp;

int main()
{
    string ip = "127.0.0.1";
    int port = 33222;
    char m1[256] = "This is message #1";
    char m2[256] = "This is message #2 ";

    async_server s;
    io_service service;
    s.bind(ip.c_str(), port);
    s.listen();

    size_t need1 = 19;
    size_t need2 = 20;

    async_socket c;
    function<void(int)> cliFunc1 = [&](int fd) {
        cerr << "Connection established " << fd << "!\n";

        function<void(int)> cliFunc2 = [&](int fd1){
            cerr << "Message #1 written!\n";

            function<void(int, void*)> cliFunc3 = [&](int fd2, void* buf){
                cerr << "Message #2 read:  " << (char*)buf << "\n";
                service.stop();
            };
            c.read(&service, need2, cliFunc3);
        };
        c.write(&service, m1, need1, cliFunc2);
    };
    c.set_connection(&service, ip.c_str(), port, cliFunc1);

    auto servFunc1 = [&](int fd) {
        async_socket client(fd);
        cerr << "Client accepted " << fd << "!\n";

        function<void(int, void*)> servFunc2 = [&](int fd1, void* buf2) {
            async_socket client2(fd1);
             cerr << "Message #1 read: " << (char*)buf2 << "\n";

             auto servFunc3 = [&](int fd2){
                cerr << "Message #2 written!\n";
             };
             client2.write(&service, m2, need2, servFunc3);
        };
        client.read(&service, need1, servFunc2);
    };

    s.get_connection(&service, servFunc1);
    service.run();
    return 0;
}