#include <printf.h>
#include "http_headers.h"
using namespace http;

http_headers::http_headers() {}

void http_headers::add_header(std::string name, std::string string) {
    header_map[name] = string;
    header_list.push_back(name);
}

void http_headers::remove_header(std::string name) {
    header_map.erase(name);
    std::vector<std::string>::iterator it = header_list.begin();
    for (; it != header_list.end() ; ++it) {
        if ((*it) == name) {
            header_list.erase(it);
            break;
        }
    }
}

std::string http_headers::get() {
    std::string result = "";
    for (int i = 0; i < header_list.size(); ++i) {
        result += header_list[i] + ": " + header_map[header_list[i]] + "\r\n";
    }
    result +=  "\r\n";
    return result;
}

bool http_headers::is_there(std::string string) {
    return header_map.count(string) > 0;
}

std::string http_headers::get_valuse(std::string string) {
    if (is_there(string))
        return header_map[string];
    else
        return "*/*";
}

void http_headers::add_header(std::string string) {
    unsigned long idx = string.find(':');
    idx += 2;
    add_header(string.substr(0, idx), string.substr(idx, string.size() - idx));
}

size_t http_headers::size() {
    return header_map.size();
}
