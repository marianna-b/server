#ifndef HTTP_SERVER_HANDLER
#define HTTP_SERVER_HANDLER

#include <functional>
#include <set>
#include <map>
#include "http_response.h"
#include "http_request.h"

namespace http {
    struct http_request_handler {

        http_request_handler();

        void set_error_handler(std::function<bool(int)>);
        void set(method_name, std::function<http::http_response(http::http_request, bool)>, bool);

        std::function<http::http_response(http::http_request, bool)> get(method_name);
        bool is_on_some(method_name);
        bool is_implemented(method_name);

        bool run_error_handler(int);

    private:
        std::map <http::method_name, std::function<http::http_response(http::http_request, bool)> > handler_map;
        bool on_some[10];
        std::function<bool(int)> error_handler = [](int i){ return i != 0; };
    };
}

#endif //HTTP_SERVER_HANDLER

