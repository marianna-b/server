#include <iostream>
#include <async_service/io_service.h>
#include <http/http_request_handler.h>
#include <http/http_server.h>

using namespace std;
using namespace tcp;
using namespace http;

int main()
{
    function<http_response(http_request)> get = [&](http_request request) {
        return http_response();
    };

    string ip = "127.0.0.1";
    int port = 44455;

    http_request_handler h(get, get, [](){});
    http_server server(ip.c_str(), port, h);
    server.start();
    return 0;
}