#include "http_request_handler.h"
using namespace http;
using namespace std;

http::http_request_handler::http_request_handler(function < http_response(http_request) > get,
        function < http_response(http_request) > head) {
    implemented.insert(GET);
    do_get = get;

    implemented.insert(HEAD);
    do_head = head;
}

void http_request_handler::set_post(function<http_response(http_request)> f) {
    implemented.insert(POST);
    do_post = f;
}

void http_request_handler::set_put(function<http_response(http_request)> f) {

    implemented.insert(PUT);
    do_put = f;
}

void http_request_handler::set_trace(function<http_response(http_request)> f) {

    implemented.insert(TRACE);
    do_trace = f;
}

void http_request_handler::set_options(function<http_response(http_request)> f) {
    implemented.insert(OPTIONS);
    do_options = f;
}

void http_request_handler::set_delete(function<http_response(http_request)> f) {
    implemented.insert(DELETE);
    do_delete = f;
}

std::function<http::http_response(http::http_request)> http_request_handler::get(method_name name) {
    switch (name) {
        case GET:
            return do_get;
        case HEAD:
            return do_head;
        case POST:
            return do_post;
        case PUT:
            return do_put;
        case TRACE:
            return do_trace;
        case OPTIONS:
            return do_options;
        case DELETE:
            return do_delete;
        case UNDEFINED:
            throw logic_error("get responce for undefined request");
    }
}
