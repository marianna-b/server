#include <string.h>
#include <iostream>
#include "http_client.h"
using namespace tcp;
using namespace http;


http_client::http_client() {
    service = new io_service;

    http_client::on_accept = [&](int error, async_socket *s) {
        error_handle(error);
        s->write(service, request, request_len, on_write);
        curr = "";
    };

    http_client::on_write = [&](int error, async_socket *s) {
        error_handle(error);
        s->read_some(service, 1000, on_read_some);
    };

    http_client::on_read_some = [&](int error, async_socket *s, void *buf) {
        std::string cr_lf_client = "\r\n"; // TODO split to different function
        error_handle(error); // TODO make bool
        if (parse != IN_BODY) {

            curr += std::string((char *) buf);
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
                        curr.substr(idx, curr.size() - idx);
                        if (need_body)
                            body = http_body(curr, headers.get_valuse("Content-Type"));
                        else
                            body = http_body();
                        curr = "";
                    }
                }
                idx = curr.find(cr_lf_p);
            }
        } else
        if (parse == IN_BODY && need_body) {
            char *cr_lf_p = (char *) cr_lf_client.c_str();
            unsigned long idx = curr.find(cr_lf_p);
            curr = std::string((char *) buf);
            std::cerr << curr << std::endl;

            if (idx == std::string::npos) {
                body.add(curr);

                if (curr.size() == 0) {
                    return on_exit();
                }
                if (headers.is_there("Content-Length")) {
                    int i = std::stoi(headers.get_valuse("Content-Length"), 0, 10);
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
        s->read_some(service, 1000, on_read_some);
    };
}

void http_client::to_string(http_request r) {
    headers = http_headers();
    request_len = r.get_to(request, 1000);
}

void http_client::send(char const *ip, int port, http::http_request r, std::function<void(http_response)> f) {
    client = new async_socket; // TODO move to constructor
    need_body = r.get_title().get_method().get_method_name() != HEAD;
    parse = OUT;
    to_string(r);
    client->set_connection(service, ip, port, on_accept);
    on_response = f;
    curr = "";
    service->run();
}

void http_client::error_handle(int error) {
    // TODO handle error
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
    std::cerr << "DESTRUCTIONS!!!" << std::endl; // TODO delete async_socket
}
