#include "http_connection.h"
using namespace http;

void http_connection::to_string(http_response r) {
    response = r.get_to();
}

http_connection::http_connection(tcp::async_socket *async_socket) {
    client = async_socket;
    condition = OUT;
}



