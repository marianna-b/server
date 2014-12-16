#ifndef HTTP_CONNECTION
#define HTTP_CONNECTION

#include "http_response.h"
#include "http_request.h"
#include <string>
#include <vector>
#include <map>
#include <tcp/async_server.h>
#include "http_request_handler.h"

namespace http {
    struct http_connection {

        http_connection();
        http_connection(tcp::async_socket *);

        void to_string(http_response);

        //TODO add destructor with deleting client member

        parse_condition condition;
        tcp::async_socket *client;
        char response[1000];
        size_t resp_len = 0;

        std::string request;
        method_name method; // TODO why no use
        bool need_body;
        http_request_title title;
        http_headers headers;
        http_body body;
    };

}

#endif // HTTP_CONNECTION
