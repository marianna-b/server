#include "method_name.h"


http::method::method() {
    name = UNDEFINED;
}


http::method::method(method_name n) {
    name = n;
}


http::method_name http::method::get(std::string s) {
    if (s == "GET")
        return GET;
    if (s == "HEAD")
        return HEAD;
    if (s == "POST")
        return POST;
    if (s == "PUT")
        return PUT;
    if (s == "TRACE")
        return TRACE;
    if (s == "OPTIONS")
        return OPTIONS;
    if (s == "DELETE")
        return DELETE;

    return UNDEFINED;
}

http::method::method(std::string s) {
    name = get(s);
}

std::string http::method::get() {
    switch (name) {
        case GET:
            return "GET";
        case HEAD:
            return "HEAD";
        case POST:
            return "POST";
        case PUT:
            return "PUT";
        case TRACE:
            return "TRACE";
        case OPTIONS:
            return "OPTIONS";
        case DELETE:
            return "DELETE";
        default:
            return "UNDEFINED";
    }
}



