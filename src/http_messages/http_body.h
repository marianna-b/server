#ifndef HTTP_BODY
#define HTTP_BODY

#include "method_name.h"
#include "mime_type.h"
#include <map>

namespace http {

    struct http_body {
        http_body();
        http_body(std::string, std::string);
        http_body(std::string);

        std::string get();

        bool is_empty();
        size_t size();
        void add(std::string);

    private:
        size_t length;
        std::string buf;
        mime_type type;
    };

}

#endif // HTTP_BODY