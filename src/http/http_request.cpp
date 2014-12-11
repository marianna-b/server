#include "http_request.h"

using namespace http;

http_request::http_request(http_request_title t, http_headers h, http_body b)
        : title(t), headers(h), body(b) {}

void http_request::set_title(http_request_title t)
{
    title = t;
}

void http_request::set_headers(http_headers h) {
    headers = h;
}

void http_request::set_body(http_body b) {
    body = b;
}

http_request_title http_request::get_title() {
    return title;
}

http_headers http_request::get_headers() {
    return headers;
}

http_body http_request::get_body() {
    return body;
}

bool http_request::has_body() {
    return body.is_empty();
}
