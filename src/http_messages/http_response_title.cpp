#include <iostream>
#include "http_response_title.h"
using namespace http;


http_response_title::http_response_title() :version(), status() {}

http_response_title::http_response_title(std::string s) {
    std::cerr << s << std::endl;
    unsigned long idx = s.find(' ');
    std::string curr = s.substr(0, idx);
    idx++;
    s = s.substr(idx, s.size() - idx);
    version = http_version(curr);
    std::cerr << version.get_version() << std::endl;
    idx = s.find(' ');
    idx++;
    curr = s.substr(0, idx);
    int code = std::stoi(curr);
    s = s.substr(idx, s.size() - idx);
    status = http_status(code, s);
    std::cerr << status.get_code() << " " << status.get_reason() << std::endl;
}


void http_response_title::set_status(http_status s) {
    status = s;
}

void http_response_title::set_version(http_version v) {
    version = v;
}

http_status http_response_title::get_status() {
    return status;
}

http_version http_response_title::get_version() {
    return version;
}

std::string http_response_title::get() {
    return version.get_version() + " " + std::to_string(status.get_code()) + " " + status.get_reason() + "\r\n";
}
