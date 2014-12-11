#ifndef METHOD_NAME
#define METHOD_NAME

#include <string>

namespace http {

    enum method_name {
        GET,
        HEAD,
        POST,
        PUT,
        TRACE,
        OPTIONS,
        DELETE,
        UNDEFINED
    };

    struct method {
        method();
        method(method_name);
        method(std::string);
        char const* get();
        method_name get(std::string);

    private:
        method_name name;
    };
}
#endif // METHOD_NAME