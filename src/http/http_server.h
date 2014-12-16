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
        // TODO add destructor with deleting service server handler

    private:
        std::map<tcp::async_socket*, http_connection> connection_map; // deleting async_socket in http_connection destructor

        tcp::io_service* service;
        tcp::async_server* server;
        http_request_handler* handler;

        void handle_error(int);
        std::function<void(int, tcp::async_socket*)> on_connect;
        std::function<void(int, tcp::async_socket*, void*)> on_read_some;
        void on_request(tcp::async_socket*);
        std::function<void(int, tcp::async_socket*)> on_send;
    };
}

#endif // HTTP_SERVER