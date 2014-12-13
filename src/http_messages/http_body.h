#ifndef HTTP_BODY
#define HTTP_BODY

#include "method_name.h"
#include "mime_type.h"
#include <map>

namespace http {

    struct http_body {
        http_body();
        http_body(size_t, void*, std::string);

        void* get();

        bool is_empty();
        size_t size();
        void add(size_t, void*);

    private:
        size_t length;
        char buffer[1000];
        mime_type type;
    };

}

#endif // HTTP_BODY