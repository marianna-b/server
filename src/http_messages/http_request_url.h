#ifndef HTTP_REQUEST_URL
#define HTTP_REQUEST_URL

#include "method_name.h"
#include "mime_type.h"
#include <map>

namespace http {

    struct http_request_url {
        http_request_url();

        http_request_url(std::string);

        std::string get_url();

        std::string get_path();

        std::string get_params();

        std::string get_query();

        std::string get_frag();

        void set_path(std::string);

        void set_params(std::string);

        void set_query(std::string);

        void set_frag(std::string);

    private:
        std::string path;
        std::string params;
        std::string query;
        std::string frag;
    };

}
#endif // HTTP_REQUEST_URL