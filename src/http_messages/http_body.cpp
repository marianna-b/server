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

http_body::http_body(std::string d, std::string s)
: type(s){
    length = d.size();
    buf = d;
}

http_body::http_body(std::string d) {
    length = d.size();
    buf = d;
}

void http_body::add(std::string d) {
    length += d.size();
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
