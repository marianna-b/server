#include <http/http_client.h>
#include <iostream>
#include <string>

using namespace std;
using namespace tcp;
using namespace http;

int main()
{
    string ip = "127.0.0.1";
    int port = 33224;
    http_request_title title;
    title.set_method(GET);
    title.set_url(http_request_url("/kdsjflskdjflskj"));
    title.set_version(http_version());

    http_headers headers;
    headers.add_header("Content-Length", "0");
    headers.add_header("Content-Type", "text/*");

    http_client client = http_client();
    client.send(ip.c_str(), port, http_request(title, headers, http_body()), [&](http_response){});
    return 0;
}