#include <string.h>
#include <iostream>
#include <error.h>
#include "http_server.h"

using namespace http;
using namespace tcp;

http::http_server::http_server(char const *s, int i, http::http_request_handler* h)
        : handler(h) {

    if (!h->is_implemented(GET) || !h->is_implemented(HEAD))
        throw std::logic_error("HEAD or GET not implemented");

    service = new io_service;
    server = new async_server;
    server->bind(s, i);
    server->listen();

    http_server::on_connect = [&](int error, async_socket *asyncSocket) {
        if (handle_error(error)) return;

        std::cerr << asyncSocket->get_fd() << std::endl;
        connection_map[asyncSocket] = new http_connection(asyncSocket);
        asyncSocket->read_some(service, 1000, on_read_some);
        server->get_connection(service, on_connect);
    };

    for (int i = 0; i < MAX_CONNECTIONS; ++i)
        server->get_connection(service, on_connect);

    http_server::on_send = [&](int error, async_socket *asyncSocket) {
        if (handle_error(error)) return;
        if (connection_map[asyncSocket]->condition == OUT) {
            delete asyncSocket;
            connection_map.erase(asyncSocket);
        }

        asyncSocket->read_some(service, 1000, on_read_some);
    };

    http_server::on_read_some = [&](int error, async_socket *asyncSocket, void *buf) {
        if (handle_error(error)) return;
        bool flag = false;

        connection_map[asyncSocket]->request += std::string((char *) buf);

        if (connection_map[asyncSocket]->condition != IN_BODY) {
            on_no_body_data(asyncSocket);
        }
        if (connection_map[asyncSocket]->condition == IN_BODY && connection_map[asyncSocket]->need_body) {
            flag = on_body_data(asyncSocket, std::string((char *) buf).size());
        }
        if (flag) return;

        if (connection_map[asyncSocket]->condition == IN_BODY && !connection_map[asyncSocket]->need_body)
            return on_request(asyncSocket, true);

        if (connection_map[asyncSocket]->condition == IN_BODY
                && handler->is_on_some(connection_map[asyncSocket]->title.get_method().get_method_name()))
            on_request(asyncSocket, false);
        else
            asyncSocket->read_some(service, 1000, on_read_some);
    };
}

bool http_server::on_body_data(async_socket* asyncSocket, size_t t) {

    http_connection* curr = connection_map[asyncSocket];
    std::string cr_lf_server = "\r\n";

    char *cr_lf_p = (char *) cr_lf_server.c_str();
    unsigned long idx = curr->request.find(cr_lf_p);

    if (idx == std::string::npos) {
        curr->body.add(curr->request);
        curr->request = "";
        if (curr->headers.is_there("Content-Length")) {
            int i = std::stoi(curr->headers.get_value("Content-Length"), 0, 10);
            if (i == curr->body.size()) {
                on_request(asyncSocket, true);
                return true;
            }
        }
    } else {
        curr->body.add(curr->request);
        curr->request = "";
        on_request(asyncSocket, true);
        return true;
    }
    return false;
}

void http_server::on_no_body_data(async_socket* asyncSocket) {
    std::string cr_lf_server = "\r\n";
    char *cr_lf_p = (char *) cr_lf_server.c_str();
    http_connection* curr = connection_map[asyncSocket];
    unsigned long idx = curr->request.find(cr_lf_p);

    while (idx != std::string::npos) {
        if (curr->condition == OUT) {

            curr->condition = IN_TITLE;
            curr->title = http_request_title(curr->request.substr(0, idx));
            curr->need_body = curr->title.get_method().get_method_name() == PUT
                    || curr->title.get_method().get_method_name() == POST;

            idx += 2;
            curr->request = curr->request.substr(idx, curr->request.size() - idx);

        } else if (curr->condition == IN_TITLE || curr->condition == IN_HEADER) {

            curr->condition = IN_HEADER;
            if (idx != 0) {
                curr->headers.add_header(curr->request.substr(0, idx));
                idx += 2;
                curr->request = curr->request.substr(idx, curr->request.size() - idx);
            } else {
                idx += 2;
                curr->request = curr->request.substr(idx, curr->request.size() - idx);
                curr->condition = IN_BODY;
                if (curr->need_body)
                    curr->body = http_body(curr->request, curr->headers.get_value("Content-Type"));
                else
                    curr->body = http_body();
                curr->request = "";
            }
        }
        idx = curr->request.find(cr_lf_p);
    }
}

void http::http_server::start() {
    service->run();
}

void http::http_server::stop() {
    service->stop();
}

bool http_server::handle_error(int error) {
    return handler->run_error_handler(error);
}

void http_server::on_request(tcp::async_socket* s, bool all) {

    auto connection = connection_map[s];
    auto request = http_request(connection->title, connection->headers, connection->body);
    auto response = handler->get(connection->title.get_method().get_method_name())(request, all);

    connection->to_string(response);
    connection->client->write(service, connection->response, connection->resp_len, on_send);
    if (all)
        connection->condition = OUT;
}

http_server::~http_server() {
    delete server;
    delete service;

    std::map<async_socket*, http_connection*>::iterator it = connection_map.begin();
    for (; it != connection_map.end(); ++it) {
        http_connection* connection = (*it).second;
        delete connection;
    }
}


