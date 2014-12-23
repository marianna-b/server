#include <iostream>
#include <fstream>
#include <sstream>
#include <async_service/signal_handler.h>
#include "messenger.h"

using namespace http;
using namespace std;

messenger::messenger() {
    messenger::get = [&](http_request r, bool) {
        cerr << r.get_title().get() + r.get_headers().get() + r.get_body().get() << endl;
        http_response response;


        if (r.get_title().get_url().get_path() == "/login.html")
            response = get_login_page();
        else if (r.get_title().get_url().get_path() == "/messenger.html") {
            string name = r.get_title().get_url().get_query();
            unsigned long idx = name.find('&');
            if (idx == string::npos) {
                //TODO handle invalid query
            }

            string lastname = name.substr(idx + 10, name.size() - idx - 10);
            name = name.substr(5, idx - 5);
            // TODO check is login


            response = get_messenger_page(name, lastname);
        } else {
            http_response response1;
            return response1;
        }
        cerr << response.get_title().get() + response.get_headers().get() + response.get_body().get() << endl;
        return response;
    };
    messenger::head = [&](http_request r, bool) {
        http_response response = get(r, true);
        response.set_body(http_body());
        return response;
    };
    messenger::post = [&](http_request r, bool) {
        cerr << r.get_title().get() + r.get_headers().get() + r.get_body().get() << endl;
        string s = r.get_body().get();

        string name = r.get_title().get_url().get_query();
        unsigned long idx = name.find('&');
        if (idx == string::npos) {
            //TODO handle invalid query
        }

        string lastname = name.substr(idx + 10, name.size() - idx - 10);
        name = name.substr(5, idx - 5);
        // TODO check is login
        messages.push_back(parse_post(s, name, lastname));

        http_response response = get_messenger_page(name, lastname);
        cerr << response.get_title().get() + response.get_headers().get() + response.get_body().get() << endl;
        return response;
    };
    messenger::error_handler = [&](int error) {
        return false; // TODO error handling
    };
    tcp::signal_handler::set();
    handler = new http_request_handler();
    handler->set(GET, get, false);
    handler->set(HEAD, head, false);
    handler->set(POST, post, false);
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

http_response messenger::get_login_page() {
    http_response_title title = http_response_title();
    title.set_status(http_status(200, "OK"));
    http_headers headers;
    headers.add_header("Content-Type", "text/html");


    string b = "";
    string line;
    ifstream myfile("/home/mariashka/work/server/src/messenger/login.html");
    if (myfile.is_open()) {
        while (getline(myfile, line))
            b = b + line + "\n";
        myfile.close();
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

    string b = "<!DOCTYPE html><html><body>\n";
    b = b + "<h2><u>" + "You are: " + name + " " + lastname + "</u></h2>";
    string line;
    ifstream myfile("/home/mariashka/work/server/src/messenger/messenger.html");
    if (myfile.is_open()) {
        while (getline(myfile, line))
            b = b + line + "\n";
        myfile.close();
    }

    for (int i = 1; i <= messages.size(); ++i) {
        b += messages[messages.size() - i];
    }
    ifstream myfile2("messenger/messenger_ending.html");
    if (myfile2.is_open()) {
        while (getline(myfile2, line))
            b = b + line + "\n";
        myfile2.close();
    }

    headers.add_header("Content-Length", to_string(b.size()));

    http_body body(b, "text/html");
    return http_response(title, headers, body);
}

string messenger::parse_post(string string1, string name, string lastname) {
    stringstream stream(string1);
    string segment;
    vector<string> seglist;
    int f = 0;
    while (getline(stream, segment, '\n')) {
        if (f >= 3)
            seglist.push_back(segment);
        f++;
    }
    seglist.pop_back();

    string result = "<h3>" + name + " " + lastname + "</h3><p>\n";
    for (int i = 0; i < seglist.size(); ++i) {
        result += seglist[i];
        if (i < seglist.size() - 1)
            result += "\n<br>\n";
    }
    result += "</p> <hr>\n";
    cerr << result << endl;
    return result;
}
