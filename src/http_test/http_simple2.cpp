#include <http/http_client.h>
#include <iostream>
#include <string>

using namespace std; using namespace tcp; using namespace http;

int main() {
    string ip = "127.0.0.1";
    int port = 33335;
    http_request_title title;
    title.set_method(GET);
    title.set_url(http_request_url("/kdsjflskdjflskj"));
    title.set_version(http_version());

    http_headers headers;
    headers.add_header("kdjfskjfwekjf", "eruehufew");
    headers.add_header("jdfhks", "fjfjfjfjf");
    headers.add_header("Content-Type", "text/*");
    http_client client = http_client(ip.c_str(), port);
    client.send(http_request(title, headers, http_body()), [&](http_response r) {
        std::cerr << r.get_title().get() + r.get_headers().get() + r.get_body().get() << endl;
    });
    return 0;
}