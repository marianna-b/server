#ifndef HTTP_VERSION
#define HTTP_VERSION

#include "method_name.h"
#include "mime_type.h"
#include <map>

namespace http {
    struct http_version {
        http_version();
        http_version(int, int);
        http_version(std::string);

        std::string get_version();

        bool operator <(http_version const&) const;
        bool operator <=(http_version const&) const;
    private:
        int x, y;
    };
}
#endif // HTTP_VERSION
