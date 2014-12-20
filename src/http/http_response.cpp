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

std::vector <char> http_response::get_to() {
     std::vector <char> v;

    std::string curr = get_title().get();
    for (int i = 0; i < curr.size(); ++i)
        v.push_back(curr[i]);

    curr = get_headers().get();
    for (int i = 0; i < curr.size(); ++i)
        v.push_back(curr[i]);

    curr = get_body().get();
    for (int i = 0; i < curr.size(); ++i)
        v.push_back(curr[i]);

    return v;
}
