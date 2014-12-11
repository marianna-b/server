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

http_body::http_body(size_t t, void *pVoid, std::string s)
: type(s){
    length = t;
    ::memcpy(buffer, pVoid, t);
}

void http_body::add(size_t t, void *pVoid) {
    ::memcpy((char*)buffer + length, buffer, t);
    length += t;
}




