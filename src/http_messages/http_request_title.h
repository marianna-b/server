#ifndef HTTP_REQUEST_TITLE
#define HTTP_REQUEST_TITLE

#include "method_name.h"
#include "mime_type.h"
#include "http_request_url.h"
#include "http_version.h"
#include <map>

namespace http {

    struct http_request_title {
        http_request_title();
        http_request_title(std::string);

        std::string get();

        void set_method(method);
        void set_url(http_request_url);
        void set_version(http_version);

        method get_method();
        http_request_url get_url();
        http_version get_version();

    private:
        method method_name;
        http_request_url url;
        http_version version;
    };
}
#endif //HTTP_REQUEST_TITLE
