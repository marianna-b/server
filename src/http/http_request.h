#ifndef HTTP_REQUEST
#define HTTP_REQUEST

#include <http_messages/http_request_title.h>
#include <http_messages/http_headers.h>
#include <http_messages/http_body.h>

namespace http {
    enum parse_condition{
        OUT,
        IN_TITLE,
        IN_HEADER,
        IN_BODY
    };

    struct http_request {
        http_request(http_request_title, http_headers, http_body);

        std::string get();
        size_t get_to(void*, size_t);

        void set_title(http_request_title);
        void set_headers(http_headers);
        void set_body(http_body);

        http_request_title get_title();
        http_headers get_headers();
        http_body get_body();
        bool has_body();

    private:
        http_request_title title;
        http_headers headers;
        http_body body;
    };

}

#endif //HTTP_REQUEST