#include <string.h>
#include "http_connection.h"
using namespace http;

void http_connection::to_string(http_response r) {
    resp_len = r.get_to(response, 1000);
}

http_connection::http_connection(tcp::async_socket *async_socket) {
    client = async_socket;
    condition = OUT;
}

http_connection::http_connection() {
}
