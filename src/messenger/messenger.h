#ifndef MESSENGER
#define MESSENGER

#include <http/http_request.h>
#include <http/http_response.h>
#include <functional>
#include <http/http_server.h>
#include <memory>
struct messenger {
    messenger();

    void start();

    void stop();

    ~messenger();

private:
    http::http_response get_messenger_page(std::string, std::string);
    std::string wrap_message(std::string, std::string, std::string);
    std::string begin_messenger(std::string, std::string);
    std::string end_messenger();

    bool valid_person(std::string);
    std::pair<std::string, std::string> get_denotation(std::string);
    http::http_response invalid_person();

    http::http_response login_response();
    http::http_response messenger_response(std::string);
    http::http_response not_found_response();

    http::http_response post_response(std::string, std::string);
    http::http_response invalid_body();
    bool valid_body(std::string);

    std::unique_ptr<http::http_request_handler> handler;
    std::unique_ptr<http::http_server> server;

    std::vector<std::string> messages;

    const std::string ip = "127.0.0.1";
    const int port = 33334;

    const std::string messenger1 = "/home/mariashka/work/server/src/messenger/messenger.html";
    const std::string messenger2 = "/home/mariashka/work/server/src/messenger/messenger_ending.html";
    const std::string login_adr = "/home/mariashka/work/server/src/messenger/login.html";

    std::function<http::http_response(http::http_request, bool)> get;
    std::function<http::http_response(http::http_request, bool)> head;
    std::function<http::http_response(http::http_request, bool)> post;
    std::function<bool(int)> error_handler;
};

#endif // MESSENGER