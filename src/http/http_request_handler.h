#ifndef HTTP_SERVER_HANDLER
#define HTTP_SERVER_HANDLER

#include <functional>
#include <set>
#include "http_response.h"
#include "http_request.h"

namespace http {
    struct http_request_handler {

        http_request_handler(std::function<http::http_response(http::http_request)>,
                std::function<http::http_response(http::http_request)>); // set get and head
        void set_post(std::function<http::http_response(http::http_request)>);
        void set_put(std::function<http::http_response(http::http_request)>);
        void set_trace(std::function<http::http_response(http::http_request)>);
        void set_options(std::function<http::http_response(http::http_request)>);
        void set_delete(std::function<http::http_response(http::http_request)>);

        std::function<http::http_response(http::http_request)> get(method_name);

    private:
        std::set <http::method_name> implemented;
        std::function<http::http_response(http::http_request)> do_get;
        std::function<http::http_response(http::http_request)> do_head;
        std::function<http::http_response(http::http_request)> do_post;
        std::function<http::http_response(http::http_request)> do_put;
        std::function<http::http_response(http::http_request)> do_trace;
        std::function<http::http_response(http::http_request)> do_options;
        std::function<http::http_response(http::http_request)> do_delete;
    };
}

#endif //HTTP_SERVER_HANDLER

