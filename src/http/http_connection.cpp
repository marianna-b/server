#include <string.h>
#include "http_connection.h"
using namespace http;

void http_connection::to_string(http_response r) {
    resp_len = 0;
    headers = http_headers();

    std::string curr = r.get_title().get();
    ::memcpy(response, curr.c_str(), curr.length());
    resp_len += curr.length();

    curr = r.get_headers().get();
    ::memcpy(response + resp_len, curr.c_str(), curr.length());
    resp_len += curr.length();

    ::memcpy(response + resp_len, r.get_body().get().c_str(), r.get_body().size());
    resp_len += r.get_body().size();
}

http_connection::http_connection(tcp::async_socket *async_socket) {
    client = async_socket;
    condition = OUT;
}

http_connection::http_connection() {
}
