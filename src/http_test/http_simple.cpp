#include <iostream>
#include <async_service/io_service.h>
#include <http/http_request_handler.h>
#include <http/http_server.h>

using namespace std;
using namespace tcp;
using namespace http;

int main()
{
    auto get = [&](http_request request, bool all) {
        if (!all) {
            std::cerr << request.get_title().get() + request.get_headers().get() + request.get_body().get() << endl;
            http_response_title title;
            title.set_status(http_status(200, "OK"));

            http_headers headers;
            std::string b = "Great!";

            headers.add_header("Content-Type", "text/*");
            http_body body(b, "text/*");

            return http_response(title, headers, body);
        } else {
            std::cerr << request.get_title().get() + request.get_headers().get() + request.get_body().get() << endl;
            http_response_title title;

            http_headers headers;
            std::string b = "\nOMNOMNOM!\r\n";
            http_body body(b, "text/*");

            return http_response(title, headers, body);
        }
    };

    string ip = "127.0.0.1";
    int port = 33334;

    http_request_handler* h = new http_request_handler();
    h->set(GET, get, false);
    h->set(HEAD, get, false);
    h->set(PUT, get, true);
    http_server server(ip.c_str(), port, h);
    server.start();
    return 0;
}