#include <string.h>
#include <iostream>
#include "http_client.h"
using namespace tcp;
using namespace http;


http_client::http_client(char const *ip, int port) {
    service = new io_service;
    client = new async_socket;

    http_client::on_accept = [&](int error, async_socket *s) {
        if (handle_error(error)) return;
        accepted = true;
        s->write(service, request, request_len, on_write);
        curr = "";
    };

    client->set_connection(service, ip, port, on_accept);

    http_client::on_write = [&](int error, async_socket *s) {
        if (handle_error(error)) return;
        s->read_some(service, MAX_BUFFER_SIZE, on_read_some);
    };

    http_client::on_read_some = [&](int error, async_socket *s, size_t size, void *buf) {
        if (handle_error(error)) return;
        bool flag = false;
        curr += std::string((char *) buf, (char *) buf + size);
        if (parse != IN_BODY) {
            on_no_body_data();
        }
        if (parse == IN_BODY && need_body) {
            flag = on_body_data(size);
        }
        if (flag) return;

        if (parse == IN_BODY && !need_body) {
            on_exit(true);
        }
        if (parse == IN_BODY)
            on_exit(false);
        //s->read_some(service, MAX_BUFFER_SIZE, on_read_some);
    };
}


bool http_client::on_body_data(size_t t) {
    std::string cr_lf_client = "\r\n";
    char *cr_lf_p = (char *) cr_lf_client.c_str();
    unsigned long idx = curr.find(cr_lf_p);

    if (idx == std::string::npos) {
        body.add(curr);

        if (t == 0) {
            on_exit(true);
            return true;
        }
        if (headers.is_there("Content-Length")) {
            int i = std::stoi(headers.get_value("Content-Length"), 0, 10);
            if (i == body.size()) {
                on_exit(true);
                return true;
            }
        }
    } else {
        std::string s2 = curr.substr(0, idx);
        body.add(s2);
        curr = "";
        on_exit(true);
        return true;
    }
    curr = "";
    return false;
}


void http_client::on_no_body_data() {
    std::string cr_lf_client = "\r\n";
    char *cr_lf_p = (char *) cr_lf_client.c_str();
    unsigned long idx = curr.find(cr_lf_p);

    while (idx != std::string::npos) {
        if (parse == OUT) {

            parse = IN_TITLE;
            title = http_response_title(curr.substr(0, idx));

            int code = title.get_status().get_code();
            need_body = !(code == 204 || code == 304 || code / 100 == 1);

            idx += 2;
            curr = curr.substr(idx, curr.size() - idx);

        } else if (parse == IN_TITLE || parse == IN_HEADER) {

            parse = IN_HEADER;
            if (idx != 0) {
                headers.add_header(curr.substr(0, idx));
                idx += 2;
                curr = curr.substr(idx, curr.size() - idx);
            } else {
                parse = IN_BODY;
                idx += 2;
                curr = curr.substr(idx, curr.size() - idx);
                if (need_body)
                    body = http_body(curr, headers.get_value("Content-Type"));
                else
                    body = http_body();
                curr = "";
            }
        }
        idx = curr.find(cr_lf_p);
    }
}


void http_client::to_string(http_request r) {
    headers = http_headers();
    request_len = r.get_to(request, MAX_BUFFER_SIZE);
}

void http_client::send(http::http_request r, std::function<void(http_response, bool)> f) {
    need_body = r.get_title().get_method().get_method_name() != HEAD;
    parse = OUT;
    to_string(r);
    on_response = f;
    if (accepted) {
        client->write(service, request, request_len, on_write);
        parse = IN_BODY;
    }
    curr = "";
    service->run();
}

bool http_client::handle_error(int error) {
    return false;
}

void http_client::on_exit(bool all) {
    http_response response(title, headers, body);
    headers = http_headers();
    curr = "";
    memset(request, 0, sizeof request);
    service->pause();
    on_response(response, all);
}


http_client::~http_client() {
    delete client;
}


