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
        s->write(service, request, request_len, on_write);
        curr = "";
    };

    client->set_connection(service, ip, port, on_accept);

    http_client::on_write = [&](int error, async_socket *s) {
        if (handle_error(error)) return;
        s->read_some(service, 1000, on_read_some);
    };

    http_client::on_read_some = [&](int error, async_socket *s, void *buf) {
        if (handle_error(error)) return;

        if (parse != IN_BODY) {
            curr += std::string((char *) buf);
            on_no_body_data();
        }
        if (parse == IN_BODY && need_body) {
            on_body_data(std::string((char *) buf).size());
        }
        s->read_some(service, 1000, on_read_some);
    };
}


void http_client::on_body_data(size_t t) {
    std::string cr_lf_client = "\r\n";
    char *cr_lf_p = (char *) cr_lf_client.c_str();
    unsigned long idx = curr.find(cr_lf_p);

    if (idx == std::string::npos) {
        body.add(curr);

        if (t == 0) {
            return on_exit();
        }
        if (headers.is_there("Content-Length")) {
            int i = std::stoi(headers.get_valuse("Content-Length"), 0, 10);
            std::cerr << body.size() << std::endl;
            std::cerr << body.get() << std::endl;
            if (i == body.size()) {
                return on_exit();
            }
        }
    } else {
        std::string s2 = curr.substr(0, idx);
        body.add(s2);
        curr = "";
        curr = curr.substr(idx, curr.size() - idx);
        return on_exit();
    }
    curr = "";
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
                std::cerr << curr << std::endl;
                idx += 2;
                curr = curr.substr(idx, curr.size() - idx);
                std::cerr << curr << std::endl;
                if (need_body)
                    body = http_body(curr, headers.get_valuse("Content-Type"));
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
    request_len = r.get_to(request, 1000);
}

void http_client::send(http::http_request r, std::function<void(http_response)> f) {
    need_body = r.get_title().get_method().get_method_name() != HEAD;
    parse = OUT;
    to_string(r);
    on_response = f;
    curr = "";
    service->run();
}

bool http_client::handle_error(int error) {
    // TODO handle error
    return false;
}

void http_client::on_exit(){
    http_response response(title, headers, body);
    headers = http_headers();
    curr = "";
    memset(request, 0, sizeof request);
    service->stop();
    on_response(response);
}


http_client::~http_client() {
    delete client;
}


