#include <iostream>
#include <string>
#include <async_service/io_service.h>
#include <tcp/async_server.h>

using namespace std;
using namespace tcp;

int main()
{
    std::string crlf = "\r\n";
    std::string curr= "\r\newhrlkwqejhrlkwqjehrlkwjehrjfda;kjfd;walefja;wfjw;fjw;oefj;wlefkfj;wljfe;lew";
    int idx = (int) curr.find(crlf.c_str());
    cerr << idx << endl;
    cerr << curr.substr(idx + 2, curr.size() - idx - 2) << endl;


    string ip = "127.0.0.1";
    int port = 33224;
    char m1[256] = "This is message #1";
    char m2[256] = "This is message #2 ";

    async_server* s = new async_server();
    io_service service;
    s->bind(ip.c_str(), port);
    s->listen();

    size_t need1 = 19;
    size_t need2 = 20;

    async_socket* c = new async_socket();
    async_socket* cs = new async_socket();

    function<void(std::string, async_socket*)> cliFunc1 = [&](std::string, async_socket* client) {
        cerr << "Connection established " << client->get_fd() << "!\n";

        function<void(std::string, async_socket*)> cliFunc2 = [&](std::string, async_socket* client2){
            cerr << "Message #1 written!\n";

            function<void(std::string, async_socket*, void*)> cliFunc3 = [&](std::string, async_socket* client3, void* buf){
                cerr << "Message #2 read:  " << (char*)buf << "\n";
                service.stop();
            };
            c->read(&service, need2, cliFunc3);
        };
        c->write(&service, m1, need1, cliFunc2);
    };
    c->set_connection(&service, ip.c_str(), port, cliFunc1);

    function<void(std::string, async_socket*)> servFunc1 = [&](std::string, async_socket* client) {
        cs = client;
        cerr << "Client accepted " << client->get_fd() << "!\n";

        function<void(std::string, async_socket*, void*)> servFunc2 = [&](std::string, async_socket* client2, void* buf2) {
             cerr << "Message #1 read: " << (char*)buf2 << "\n";

             function<void(std::string, async_socket*)> servFunc3 = [&](std::string, async_socket* client3){
                cerr << "Message #2 written!\n";
             };
             client2->write(&service, m2, need2, servFunc3);
        };
        client->read(&service, need1, servFunc2);
    };

    s->get_connection(&service, servFunc1);
    service.run();
    delete(s);
    delete(c);
    delete(cs);
    return 0;
}