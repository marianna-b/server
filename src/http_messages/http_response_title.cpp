#include <iostream>
#include "http_response_title.h"
using namespace http;


http_response_title::http_response_title() :version(), status() {
    empty = true;
}

http_response_title::http_response_title(std::string s) {
    empty = false;
    unsigned long idx = s.find(' ');
    std::string curr = s.substr(0, idx);

    idx++;
    s = s.substr(idx, s.size() - idx);
    version = http_version(curr);

    idx = s.find(' ');
    curr = s.substr(0, idx);
    int code = std::stoi(curr);

    idx++;
    curr = s.substr(idx, s.size() - idx);
    status = http_status(code, curr);

    std::cerr << status.get_code() << " " << status.get_reason() << std::endl;
}


void http_response_title::set_status(http_status s) {
    status = s;
    empty = false;
}

void http_response_title::set_version(http_version v) {
    version = v;
    empty = false;
}

http_status http_response_title::get_status() {
    return status;
}

http_version http_response_title::get_version() {
    return version;
}

std::string http_response_title::get() {
    if (empty)
        return "";
    return version.get_version() + " " + std::to_string(status.get_code()) + " " + status.get_reason() + "\r\n";
}
