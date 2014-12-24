#include <iostream>
#include <fstream>
#include <sstream>
#include <async_service/signal_handler.h>
#include "messenger.h"

using namespace http;
using namespace std;

messenger::messenger() {
    messenger::error_handler = [&](int error) {
        return error != 0;
    };
    messenger::get = [&](http_request r, bool) {
        try {
            string path = r.get_title().get_url().get_path();
            string query = r.get_title().get_url().get_query();

            if (path == "/login.html") {
                return login_response();
            } else if (path == "/messenger.html") {
                return messenger_response(query);
            } else {
                return not_found_response();
            }
        } catch (exception e) {
            return internal_error();
        }
    };
    messenger::head = [&](http_request r, bool) {
        try {
            http_response response = get(r, true);
            response.set_body(http_body());
            return response;
        } catch (exception e) {
            return internal_error();
        }
    };
    messenger::post = [&](http_request r, bool) {
        try {
            string body = r.get_body().get();
            string query = r.get_title().get_url().get_query();

            return post_response(body, query);
        } catch (exception e) {
            return internal_error();
        }
    };
    tcp::signal_handler::set();
    handler = new http_request_handler();
    handler->set(GET, get, false);
    handler->set(HEAD, head, false);
    handler->set(POST, post, false);
    handler->set_error_handler(error_handler);
    server = new http_server(ip.c_str(), port, handler);
};

messenger::~messenger() {
    delete server;
    delete handler;
}

void messenger::start() {
    server->start();
}

void messenger::stop() {
    server->stop();
}

http_response messenger::login_response() {
    http_response_title title = http_response_title();
    title.set_status(http_status(200, "OK"));
    http_headers headers;
    headers.add_header("Content-Type", "text/html");

    string b = "";
    string line;
    ifstream login(login_adr);
    if (login.is_open()) {
        while (getline(login, line))
            b = b + line + "\n";
        login.close();
    }

    headers.add_header("Content-Length", to_string(b.size()));
    http_body body(b, "text/html");
    return http_response(title, headers, body);
}

http_response messenger::get_messenger_page(string name, string lastname) {
    http_response_title title = http_response_title();
    title.set_status(http_status(200, "OK"));
    http_headers headers;
    headers.add_header("Content-Type", "text/html");

    string b = begin_messenger(name, lastname);
    for (int i = 1; i <= messages.size(); ++i) {
        b += messages[messages.size() - i];
    }
    b += end_messenger();

    headers.add_header("Content-Length", to_string(b.size()));
    http_body body(b, "text/html");
    return http_response(title, headers, body);
}

string messenger::wrap_message(string message, string name, string lastname) {
    stringstream stream(message);
    string segment;
    vector<string> list;
    int f = 0;
    while (getline(stream, segment, '\n')) {
        if (f >= 3)
            list.push_back(segment);
        f++;
    }
    list.pop_back();

    string result = "<h3>" + name + " " + lastname + "</h3><p>\n";
    for (int i = 0; i < list.size(); ++i) {
        result += list[i];
        if (i < list.size() - 1)
            result += "\n<br>\n";
    }
    result += "</p> <hr>\n";
    return result;
}

string messenger::begin_messenger(string name, string lastname) {
    string b = "<!DOCTYPE html><html><body>\n";
    b = b + "<h2><u>" + "You are: " + name + " " + lastname + "</u></h2>";
    string line;
    ifstream messenger(messenger1);
    if (messenger.is_open()) {
        while (getline(messenger, line))
            b = b + line + "\n";
        messenger.close();
    }
    return b;
}

string messenger::end_messenger() {
    string b = "";
    string line;
    ifstream messenger_ending(messenger2);
    if (messenger_ending.is_open()) {
        while (getline(messenger_ending, line))
            b = b + line + "\n";
        messenger_ending.close();
    }
    return b;
}

bool messenger::valid_person(string query) {
    unsigned long idx = query.find('&');
    return idx != string::npos && idx != 5 && idx != (query.size() - 10);
}

pair<string, string> messenger::get_denotation(string query) { // parses correctly only correct queries
    unsigned long idx = query.find('&');
    string lastname = query.substr(idx + 10, query.size() - idx - 10);
    string name = query.substr(5, idx - 5);
    return make_pair(name, lastname);
}

http::http_response messenger::invalid_person() {
    http_response_title title;
    title.set_status(http_status(400, "Bad Request"));
    http_headers headers;
    headers.add_header("Content-Length", "15");
    headers.add_header("Content-Type", "text/plain");
    http_body body("400 Bad Request");

    return http_response(title, headers, body);
}

http::http_response messenger::messenger_response(string query) {
    if (!valid_person(query))
        return invalid_person();
    pair<string, string> d = get_denotation(query);
    return get_messenger_page(d.first, d.second);
}

http::http_response messenger::not_found_response() {
    http_response_title title;
    title.set_status(http_status(404, "Not Found"));
    http_headers headers;
    headers.add_header("Content-Length", "13");
    headers.add_header("Content-Type", "text/plain");
    http_body body("404 Not Found");
    return http_response(title, headers, body);
}

http_response messenger::internal_error() {
    http_response_title title;
    title.set_status(http_status(500, "Internal Server Error"));
    http_headers headers;
    headers.add_header("Content-Length", "25");
    headers.add_header("Content-Type", "text/plain");
    http_body body("500 Internal Server Error");
    return http_response(title, headers, body);
}

http::http_response messenger::post_response(string body, string query) {
    if (!valid_person(query))
        return invalid_person();
    pair<string, string> d = get_denotation(query);

    if (valid_body(body))
        messages.push_back(wrap_message(body, d.first, d.second));
    else
        return invalid_body();

    http_response response = get_messenger_page(d.first, d.second);
    return response;
}

bool messenger::valid_body(string message) {
    stringstream stream(message);
    string segment;
    int f = 0;
    while (getline(stream, segment, '\n')) {
        f++;
    }
    return f >= 3;
}

http::http_response messenger::invalid_body() {
    http_response_title title;
    title.set_status(http_status(400, "Bad Request"));
    http_headers headers;
    headers.add_header("Content-Length", "15");
    headers.add_header("Content-Type", "text/plain");
    http_body body("400 Bad Request");

    return http_response(title, headers, body);
}
