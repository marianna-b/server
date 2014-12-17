#include <printf.h>
#include "http_headers.h"
using namespace http;
using namespace std;

http_headers::http_headers() {
    empty = true;
}

void http_headers::add_header(string name, string string) {
    empty = false;
    header_map[name] = string;
    header_list.push_back(name);
}

void http_headers::remove_header(string name) {
    header_map.erase(name);
    vector<string>::iterator it = header_list.begin();
    for (; it != header_list.end() ; ++it) {
        if ((*it) == name) {
            header_list.erase(it);
            break;
        }
    }

    empty = header_map.size() == 0;
}

string http_headers::get() {
    string result = "";
    if (empty)
        return result;
    for (int i = 0; i < header_list.size(); ++i) {
        result += header_list[i] + ": " + header_map[header_list[i]] + "\r\n";
    }
    result +=  "\r\n";
    return result;
}

bool http_headers::is_there(string string) {
    return header_map.count(string) > 0;
}

string http_headers::get_value(string string) {
    if (is_there(string))
        return header_map[string];
    else
        return "*/*";
}

void http_headers::add_header(string string) {
    empty = false;
    unsigned long idx = string.find(':');
    add_header(string.substr(0, idx), string.substr(idx + 2, string.size() - idx - 2));
}

size_t http_headers::size() {
    return header_map.size();
}

vector<pair<string, string> > http_headers::get_list() {
    vector<pair<string, string> > v;
    for (int i = 0; i < header_list.size(); ++i) {
        v.push_back(make_pair(header_list[i], header_map[header_list[i]]));
    }
    return v;
}

void http_headers::set_not_empty() {
    empty = false;
}
