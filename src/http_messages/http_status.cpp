#include "http_status.h"
using namespace http;

http_status::http_status() {
    code = 500;
    reason = "Internal Server Error";
}

http_status::http_status(int i, std::string s) {
    code = i;
    reason = s;
}

void http_status::set_code(int i) {
    code = i;
}

void http_status::set_reason(std::string s) {
    reason = s;
}

int http_status::get_code() {
    return code;
}

std::string http_status::get_reason() {
    return reason;
}
