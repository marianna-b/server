#include "http_request_handler.h"
using namespace http;
using namespace std;

http::http_request_handler::http_request_handler(function < http_response(http_request) > get,
        function < http_response(http_request) > head, function<void()> t) {
    handler_map[GET] = get;
    handler_map[HEAD] = head;
    on_term = t;
}

std::function<http::http_response(http::http_request)> http_request_handler::get(method_name name) {
    return handler_map[name];
}

bool http_request_handler::is_implemented(method_name name) {
    return handler_map.count(name) > 0;
}


void http_request_handler::set(method_name name, function<http_response(http_request)> f) {
    handler_map[name] = f;
}

void http_request_handler::on_terminate() {
    on_term();
}
