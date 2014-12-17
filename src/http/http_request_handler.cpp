#include "http_request_handler.h"
using namespace http;
using namespace std;

std::function<http::http_response(http::http_request, bool)> http_request_handler::get(method_name name) {
    return handler_map[name];
}

bool http_request_handler::is_implemented(method_name name) {
    return handler_map.count(name) > 0;
}


void http_request_handler::set(method_name name, function<http_response(http_request, bool)> f, bool some) {
    handler_map[name] = f;
    on_some[name] = some;
}

http_request_handler::http_request_handler() {}

void http_request_handler::set_error_handler(function<bool(int)> f) {
    error_handler = f;
}

bool http_request_handler::run_error_handler(int i) {
    return error_handler(i);
}

bool http_request_handler::is_on_some(method_name name) {
    return on_some[name];
}
