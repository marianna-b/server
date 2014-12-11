#include <string>
#include "mime_type.h"

http::mime_type::mime_type(std::string s) {
    unsigned long idx = s.find('/');
    type = s.substr(0, idx);
    ext = s.substr(idx, s.size() - idx);
}
