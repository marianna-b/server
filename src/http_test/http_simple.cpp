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
        std::cerr << request.get_title().get() + request.get_headers().get() + request.get_body().get() << endl;
        http_response_title title;
        title.set_status(http_status(200, "OK"));

        http_headers headers;
        std::string b = "Great!";
        unsigned long b_s = b.size();

        headers.add_header("Content-Length", to_string(b_s));
        headers.add_header("Content-Type", "text/*");
        http_body body(b.size(), b, "text/*");

        return http_response(title, headers, body);
    };

    string ip = "127.0.0.1";
    int port = 23335;

    http_request_handler* h = new http_request_handler(get, get, [](){});
    http_server server(ip.c_str(), port, h);
    server.start();
    return 0;
}