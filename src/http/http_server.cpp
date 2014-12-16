#include <string.h>
#include <iostream>
#include "http_server.h"

using namespace http;
using namespace tcp;

http::http_server::http_server(char const *s, int i, http::http_request_handler* h)
        : handler(h) {
    service = new io_service;
    server = new async_server;
    server->bind(s, i);
    server->listen();

    http_server::on_connect = [&](int s1, async_socket *asyncSocket) {
        handle_error(s1);
        connection_map[asyncSocket] = http_connection(asyncSocket);
        asyncSocket->read_some(service, 1000, on_read_some);
        server->get_connection(service, on_connect);
    };

    for (int i = 0; i < MAX_CONNECTIONS; ++i)
        server->get_connection(service, on_connect);

    http_server::on_send = [&](int error, async_socket *asyncSocket) {
        handle_error(error);
        connection_map.erase(asyncSocket);
        delete asyncSocket;
    };


    http_server::on_read_some = [&](int error, async_socket *asyncSocket, void *buf) {
        handle_error(error); // TODO make bool

        // TODO split into different

        http_connection curr = connection_map[asyncSocket];
        std::string cr_lf_server = "\r\n";

        if (curr.condition != IN_BODY) {
            curr.request += std::string((char *) buf);
            char *cr_lf_p = (char *) cr_lf_server.c_str();
            unsigned long idx = curr.request.find(cr_lf_p);

            while (idx != std::string::npos) {
                if (curr.condition == OUT) {

                    curr.condition = IN_TITLE;
                    curr.title = http_request_title(curr.request.substr(0, idx));
                    curr.need_body = curr.title.get_method().get_method_name() == PUT
                            || curr.title.get_method().get_method_name() == POST;

                    idx += 2;
                    curr.request = curr.request.substr(idx, curr.request.size() - idx);

                } else if (curr.condition == IN_TITLE || curr.condition == IN_HEADER) {

                    curr.condition = IN_HEADER;
                    if (idx != 0) {
                        curr.headers.add_header(curr.request.substr(0, idx));
                        idx += 2;
                        curr.request = curr.request.substr(idx, curr.request.size() - idx);
                    } else {
                        idx += 2;
                        curr.request = curr.request.substr(idx, curr.request.size() - idx);
                        curr.condition = IN_BODY;
                        if (curr.need_body)
                            curr.body = http_body(curr.request, curr.headers.get_valuse("Content-Type"));
                        else
                            curr.body = http_body();
                        curr.request = "";
                    }
                }
                idx = curr.request.find(cr_lf_p);
            }
        }
        if (curr.condition == IN_BODY && curr.need_body) {
            curr.request += std::string((char *) buf);
            char *cr_lf_p = (char *) cr_lf_server.c_str();
            unsigned long idx = curr.request.find(cr_lf_p);

            if (idx == std::string::npos) {
                curr.body.add(curr.request);

                if (std::string((char *) buf).size() == 0) {
                    return handler->on_terminate(); // TODO wtf??
                }
                if (curr.headers.is_there("Content-Length")) {
                    int i = std::stoi(curr.headers.get_valuse("Content-Length"), 0, 10);
                    if (i == curr.body.size()) {
                        return on_request(asyncSocket);
                    }
                }
            } else {
                std::string s2 = curr.request.substr(0, idx);
                curr.body.add(s2);
                curr.request = curr.request.substr(idx, curr.request.size() - idx); // TODO wtf??
                return on_request(asyncSocket);
            }
        }
        connection_map[asyncSocket] = curr;
        if (curr.condition == IN_BODY && !curr.need_body)
            return on_request(asyncSocket);
        asyncSocket->read_some(service, 1000, on_read_some);
    };
}

void http::http_server::start() {
    service->run();
}

void http::http_server::stop() {
    service->stop();
}

void http_server::handle_error(int error) {
    //TODO error handling
}

void http_server::on_request(tcp::async_socket* s) {
    http_connection connection = connection_map[s];
    http_request request = http_request(connection.title, connection.headers, connection.body);
    http_response response = handler->get(connection.title.get_method().get_method_name())(request);
    connection.to_string(response);
    connection.client->write(service, connection.response, connection.resp_len, on_send);
}




