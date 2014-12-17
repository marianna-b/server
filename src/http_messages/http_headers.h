#ifndef HTTP_HEADERS
#define HTTP_HEADERS

#include "method_name.h"
#include "mime_type.h"
#include <map>
#include <vector>

namespace http {

    struct http_headers {
        http_headers();
        std::string get();

        bool is_there(std::string);
        std::string get_value(std::string);
        void add_header(std::string, std::string);
        void add_header(std::string);

        std::vector<std::pair <std::string, std::string> > get_list();

        void remove_header(std::string);
        size_t size();
        void set_not_empty();

    private:
        std::map<std::string, std::string> header_map;
        std::vector <std::string> header_list;
        bool empty;
    };
}
#endif // HTTP_HEADERS
