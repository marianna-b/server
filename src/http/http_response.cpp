#include <printf.h>
#include <glob.h>
#include <string.h>
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

std::string http_response::get() {
    return get_title().get() + get_headers().get() + get_body().get();
}

size_t http_response::get_to(void *r, size_t t) {
    char* response = (char*) r;
    ::memset(response, 0, t);
    size_t resp_len = 0;

    std::string curr = get_title().get();
    ::memcpy(response, curr.c_str(), curr.length());
    resp_len += curr.length();

    curr = get_headers().get();
    ::memcpy(response + resp_len, curr.c_str(), curr.length());
    resp_len += curr.length();

    ::memcpy(response + resp_len, get_body().get().c_str(), get_body().size());
    resp_len += get_body().size();

    return resp_len;
}
