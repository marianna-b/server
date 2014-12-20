#include "http_connection.h"
using namespace http;

void http_connection::to_string(http_response r) {
    response = r.get_to();
}

http_connection::http_connection(tcp::async_socket *async_socket) {
    no_del = false;
    client = async_socket;
    condition = OUT;
}

http_connection::~http_connection() {
    if (!no_del)
        delete client;
}

http_connection::http_connection() {
    no_del = true;
}
