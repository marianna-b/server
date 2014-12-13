#include <printf.h>
#include <string.h>
#include "http_body.h"

using namespace http;

bool http_body::is_empty() {
    return length == 0;
}

size_t http_body::size() {
    return length;
}

http_body::http_body(size_t t, std::string d, std::string s)
: type(s){
    length = t;
    buf = d;
}

void http_body::add(size_t t, std::string d) {
    length += t;
    buf += d;
}


http_body::http_body() {
    length = 0;
    buf = "";
    type = mime_type("*/*");
}

std::string http_body::get() {
    return buf;
}
