#ifndef HTTP_RESPONSE_TITLE
#define HTTP_RESPONSE_TITLE

#include "method_name.h"
#include "mime_type.h"
#include "http_status.h"
#include "http_version.h"
#include <map>

namespace http {

    struct http_response_title {
        http_response_title(); // default HTTP/1.1, 500, "Internal Server Error"
        http_response_title(std::string);

        std::string get();

        void set_status(http_status);

        void set_version(http_version);

        http_status get_status();

        http_version get_version();

    private:
        http_version version;
        http_status status;
    };
}

#endif //HTTP_RESPONSE_TITLE
