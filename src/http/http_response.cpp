#include "http_response.h"

using namespace http;

http_response::http_response(http_response_title t, http_headers h, http_body b)
        : title(t), headers(h), body(b) {}

void http_response::set_title(http_response_title t)
{
    title = t;
}

void http_response::set_headers(http_headers h) {
    headers = h;
}

void http_response::set_body(http_body b) {
    body = b;
}

http_response_title http_response::get_title() {
    return title;
}

http_headers http_response::get_headers() {
    return headers;
}

http_body http_response::get_body() {
    return body;
}

bool http_response::has_body() {
    return body.is_empty();
}

http_response::http_response() {
}
