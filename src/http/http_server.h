#ifndef HTTP_SERVER
#define HTTP_SERVER

#include <string>
#include <vector>
#include <map>
#include <tcp/async_server.h>
#include "http_request_handler.h"
#include "http_connection.h"

namespace http {


    struct http_server {
        const int MAX_CONNECTIONS = 100;

        http_server(char const*, int, http_request_handler*);
        void start();
        void stop();
        ~http_server();

    private:
        std::map<tcp::async_socket*, http_connection*> connection_map;

        tcp::io_service* service;
        tcp::async_server* server;
        http_request_handler* handler;

        http::http_response not_implemented_response();
        http::http_response internal_error();
        void connection_failed(tcp::async_socket*);
        void on_no_body_data(tcp::async_socket*);
        bool on_body_data(tcp::async_socket*, size_t);
        bool handle_error(int);
        std::function<void(int, tcp::async_socket*)> on_connect;
        std::function<void(int, tcp::async_socket*, size_t, void*)> on_read_some;
        void on_request(tcp::async_socket*, bool);
        std::function<void(int, tcp::async_socket*)> on_send;
        std::function<void(int, tcp::async_socket*)> on_write_some;
    };
}

#endif // HTTP_SERVER