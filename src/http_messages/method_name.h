#ifndef METHOD_NAME
#define METHOD_NAME

#include <string>

namespace http {

    enum method_name {
        GET = 1,
        HEAD = 2,
        POST = 3,
        PUT = 4,
        TRACE = 5,
        OPTIONS = 6,
        DELETE = 7,
        UNDEFINED = 8
    };

    struct method {
        method();
        method(method_name);
        method(std::string);
        std::string get();
        method_name get_method_name();
        method_name get(std::string);

    private:
        method_name name;
    };
}
#endif // METHOD_NAME