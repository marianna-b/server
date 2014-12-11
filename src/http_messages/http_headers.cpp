#include "http_headers.h"
using namespace http;

http_headers::http_headers() {}

void http_headers::add_header(std::string name, std::string string) {
    list[name] = string;
}

void http_headers::remove_header(std::string name) {
    list.erase(name);
}
