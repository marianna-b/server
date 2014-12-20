#ifndef HTTP_CLIENT
#define HTTP_CLIENT

#include <tcp/async_socket.h>
#include <async_service/io_service.h>
#include "http_response.h"
#include "http_request.h"

namespace http {

    struct http_client {

        http_client(char const*, int);
        void send(http_request, std::function<void(http_response, bool)>);
        ~http_client();

    private:
        bool accepted = false;
        char request[1000];
        size_t request_len;
        void to_string(http_request);
        bool need_body = true;
        std::string curr = "";

        tcp::async_socket* client;
        tcp::io_service* service;
        bool handle_error(int);
        void on_exit(bool);
        void on_no_body_data();
        bool on_body_data(size_t);

        std::function<void(int, tcp::async_socket*)> on_accept;
        std::function<void(int, tcp::async_socket*)> on_write;
        std::function<void(int, tcp::async_socket*, size_t, void*)> on_read_some;
        std::function<void(http_response, bool)> on_response;

        parse_condition parse;
        http_response_title title;
        http_headers headers;
        http_body body;
    };
}
#endif // HTTP_CLIENT
