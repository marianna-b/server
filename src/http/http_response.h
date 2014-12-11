#ifndef HTTP_RESPONSE
#define HTTP_RESPONSE

#include <http_messages/http_response_title.h>
#include <http_messages/http_headers.h>
#include <http_messages/http_body.h>

namespace http {

    struct http_response {
        http_response(http_response_title, http_headers, http_body);

        void set_title(http_response_title);
        void set_headers(http_headers);
        void set_body(http_body);

        http_response_title get_title();
        http_headers get_headers();
        http_body get_body();
        bool has_body();

    private:
        http_response_title title;
        http_headers headers;
        http_body body;
    };

}

#endif //HTTP_RESPONSE