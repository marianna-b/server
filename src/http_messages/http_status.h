#ifndef HTTP_STATUS
#define HTTP_STATUS

#include "method_name.h"
#include "mime_type.h"
#include <map>

namespace http {
    struct http_status {
        http_status();

        http_status(int, std::string);

        int get_code();

        std::string get_reason();

        void set_code(int);

        void set_reason(std::string);

    private:
        int code;
        std::string reason;
    };
}
#endif // HTTP_STATUS