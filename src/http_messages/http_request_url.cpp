#include "http_request_url.h"
using namespace http;

http_request_url::http_request_url() {
    path = "";
    params = "";
    query = "";
    frag = "";
}

http_request_url::http_request_url(std::string s) {
    unsigned long idx = s.find('#');
    if (idx != std::string::npos) {
        frag = s.substr(idx + 1, s.size() - idx - 1);
        s = s.substr(0, idx);
    }
    idx = s.find('?');
    if (idx != std::string::npos) {
        query = s.substr(idx + 1, s.size() - idx - 1);
        s = s.substr(0, idx);
    }
    idx = s.find(';');
    if (idx != std::string::npos) {
        params = s.substr(idx + 1, s.size() - idx - 1);
        s = s.substr(0, idx);
    }
    path = s;
}


std::string http_request_url::get_url() {
    std::string res = path;
    if (params.length() > 0)
        res = res + ";" + params;
    if (query.length() > 0)
        res = res + "?" + query;
    if (frag.length() > 0)
        res = res + "#" + frag;
    return res;
}

std::string http_request_url::get_path() {
    return path;
}

std::string http_request_url::get_params() {
    return params;
}

std::string http_request_url::get_query() {
    return query;
}

std::string http_request_url::get_frag() {
    return frag;
}

void http_request_url::set_path(std::string s) {
    path = s;
}

void http_request_url::set_params(std::string s) {
    params = s;
}

void http_request_url::set_query(std::string s) {
    query = s;
}

void http_request_url::set_frag(std::string s) {
    frag = s;
}
