#include <iostream>
#include "http_request_title.h"
using namespace http;


http_request_title::http_request_title()
: version(1, 1), method_name(""), empty(true) {}

http_request_title::http_request_title(std::string s) {
    empty = false;

    unsigned long idx = s.find(' ');
    std::string curr = s.substr(0, idx);
    idx++;

    s = s.substr(idx, s.size() - idx);
    method_name = method(curr);
    idx = s.find(' ');

    curr = s.substr(0, idx);
    url = http_request_url(curr);
    idx++;

    s = s.substr(idx, s.size() - idx);
    version = http_version(s);
}


void http_request_title::set_method(method methodName) {
    method_name = methodName;
    empty = false;
}

void http_request_title::set_url(http_request_url u) {
    url = u;
    empty = false;
}

void http_request_title::set_version(http_version v) {
    version = v;
    empty = false;
}

method http_request_title::get_method() {
    return method_name;
}

http_request_url http_request_title::get_url() {
    return url;
}

http_version http_request_title::get_version() {
    return version;
}

std::string http_request_title::get() {
    if (empty)
        return "";
    return method_name.get() + " " + url.get_url() + " " + version.get_version() + "\r\n";
}
