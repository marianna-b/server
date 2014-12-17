#include <http/http_client.h>
#include <iostream>
#include <string>

using namespace std; using namespace tcp; using namespace http;

int main() {
    string ip = "127.0.0.1";
    int port = 33334;

    string s = "ORORORORORORORORROROROR\r\n";

    http_request_title title;
    title.set_method(PUT);
    title.set_url(http_request_url("/kdsjflskdjflskj"));
    title.set_version(http_version());

    http_headers headers;
    headers.add_header("kdjfskjfwekjf", "eruehufew");
    headers.add_header("jdfhks", "fjfjfjfjf");
    headers.add_header("Content-Type", "text/*");

    http_client client = http_client(ip.c_str(), port);
    auto f = [&](http_response r, bool all) {
        std::cerr << r.get_title().get() + r.get_headers().get() + r.get_body().get() << endl;
    };

    client.send(http_request(title, headers, http_body()), f);
    title = http_request_title();
    headers = http_headers();
    http_body body(s);
    client.send(http_request(title, headers, body), f);
    return 0;
}