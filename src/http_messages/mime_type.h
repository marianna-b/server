#ifndef MIME_TYPE
#define MIME_TYPE
namespace http {
    struct mime_type {
        mime_type(std::string);
    private:
        std::string type;
        std::string ext;
    };
}

#endif // MIME_TYPE