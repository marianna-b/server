#include <iostream>
#include "http_version.h"

using namespace http;

http_version::http_version(int i, int j) {
    x = i;
    y = j;
}

std::string http_version::get_version() {
    std::string i = std::to_string(x);
    std::string j = std::to_string(y);
    return "HTTP/" + i + "." + j;
}


bool http_version::operator<(http_version const &aConst) const {
    return (x < aConst.x) || (x == aConst.x && y <= aConst.y);
}

bool http_version::operator<=(http_version const &aConst) const {
    return false;
}


http_version::http_version(std::string string1) {
    std:: cerr << string1 <<  std::endl;
    std::string s = string1.substr(5, string1.size() - 5);
    unsigned long idx = s.find('.');
    x = std::stoi(s.substr(0, idx));
    idx++;
    y = std::stoi(s.substr(idx, s.size() - idx));
}

http_version::http_version() {
    x = 1;
    y = 1;
}
