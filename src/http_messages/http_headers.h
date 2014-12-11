#ifndef HTTP_HEADERS
#define HTTP_HEADERS

#include "method_name.h"
#include "mime_type.h"
#include <map>

namespace http {

    struct http_headers {
        http_headers();

        void add_header(std::string, std::string);

        void remove_header(std::string);

    private:
        std::map<std::string, std::string> list;
    };
}
#endif // HTTP_HEADERS
