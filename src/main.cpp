#include "tcp/socket.h"
#include "tcp/server.h"
#include <thread>
#include <iostream>

using namespace std;
using namespace tcp;

int main()
{
    server s;
    string ip = "127.0.0.1";
    int port = 32322;

    string msg = "This shit fucking works!\n";
    string msg2 = "This shit fucking really works!\n";

    s.bind(ip.c_str(), port);

    std::thread t([&]() {
        client c;
        c.set_connection(ip.c_str(), port);
        c.write(msg.c_str(), msg.length() + 1);

        char buf2[256];
        c.read(buf2, msg2.length() + 1);
        cerr << buf2;
    });

    client* c2 = s.get_connection();
    char buf[256];
    c2 -> read(buf, msg.length() + 1);
    cerr << buf;

    c2->write(msg2.c_str(), msg2.length() + 1);
    t.join();
    return 0;
}