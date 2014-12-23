#ifndef MESSENGER
#define MESSENGER

#include <http/http_request.h>
#include <http/http_response.h>
#include <functional>
#include <http/http_server.h>

struct messenger {
    messenger();

    void start();

    void stop();

    ~messenger();

private:
    http::http_response get_messenger_page(std::string, std::string);
    http::http_response get_login_page();
    std::string parse_post(std::string, std::string, std::string);

    std::function<http::http_response(http::http_request, bool)> get;
    std::function<http::http_response(http::http_request, bool)> head;
    std::function<http::http_response(http::http_request, bool)> post;

    std::function<bool(int)> error_handler;

    http::http_request_handler *handler;

    std::vector<std::string> messages;

    http::http_server *server;
    std::string ip = "127.0.0.1";
    int port = 33336;
};

#endif // MESSENGER