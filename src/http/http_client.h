#ifndef HTTP_CLIENT
#define HTTP_CLIENT

#include <tcp/async_socket.h>
#include <async_service/io_service.h>
#include "http_response.h"
#include "http_request.h"

namespace http {

    struct http_client {

        http_client();
        http_response send(char const*, int, http_request, std::function<void(http_response)>);

    private:
        char request[1000];
        size_t request_len;
        void to_string(http_request);
        bool need_body = true;
        std::string curr = "";

        tcp::async_socket* client;
        tcp::io_service* service;
        void error_handle(std::string);
        void on_exit();

        std::function<void(std::string, tcp::async_socket*)> on_accept;
        std::function<void(std::string, tcp::async_socket*)> on_write;
        std::function<void(std::string, tcp::async_socket*, void*)> on_read_some;
        std::function<void(http_response)> on_response;

        parse_condition parse;
        http_response_title title;
        http_headers headers;
        http_body body;
    };
}
#endif // HTTP_CLIENT
