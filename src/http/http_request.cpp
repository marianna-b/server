#include <string.h>
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

std::string http_request::get() {
    return get_title().get() + get_headers().get() + get_body().get();
}

size_t http_request::get_to(void *r, size_t size) {
    char* request = (char*)r;
    ::memset(request, 0, size);
    size_t request_len = 0;

    std::string curr = get_title().get();
    ::memcpy(request, curr.c_str(), curr.length());
    request_len += curr.length();

    curr = get_headers().get();
    ::memcpy(request + request_len, curr.c_str(), curr.length());
    request_len += curr.length();

    ::memcpy(request + request_len, get_body().get().c_str(), get_body().size());
    request_len += get_body().size();

    return request_len;
}
