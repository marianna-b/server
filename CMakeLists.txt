cmake_minimum_required(VERSION 2.8.4)
project(tcp_server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

include_directories(src)
set(SOURCES
                    src/tcp/async_socket.h
                    src/tcp/async_socket.cpp
                    src/tcp/async_server.h
                    src/tcp/async_server.cpp
                    src/async_service/epoll.h
                    src/async_service/epoll.cpp
                    src/async_service/io_service.h
                    src/async_service/io_service.cpp
                    src/async_service/io_events.h
                    src/async_service/io_events.cpp
                    src/async_service/signal_handler.h
                    src/async_service/signal_handler.cpp
                    src/http/http_client.h
                    src/http/http_client.cpp
                    src/http/http_connection.h
                    src/http/http_connection.cpp
                    src/http/http_server.h
                    src/http/http_server.cpp
                    src/http_messages/http_body.h
                    src/http_messages/http_body.cpp
                    src/http_messages/http_request_url.h
                    src/http_messages/http_request_url.cpp
                    src/http_messages/http_version.h
                    src/http_messages/http_version.cpp
                    src/http_messages/http_status.h
                    src/http_messages/http_status.cpp
                    src/http_messages/http_response_title.h
                    src/http_messages/http_response_title.cpp
                    src/http_messages/http_request_title.h
                    src/http_messages/http_request_title.cpp
                    src/http_messages/http_headers.h
                    src/http_messages/http_headers.cpp
                    src/http/http_request.h
                    src/http/http_request.cpp
                    src/http/http_response.h
                    src/http/http_response.cpp
                    src/http/http_request_handler.h
                    src/http/http_request_handler.cpp
                    src/http_messages/method_name.h
                    src/http_messages/method_name.cpp
                    src/http_messages/mime_type.h
                    src/http_messages/mime_type.cpp
                    )

#add_executable(async_test ${SOURCES} src/socket_tests/async_test.cpp)
#add_executable(echo_client ${SOURCES} src/socket_tests/echo_client.cpp)
#add_executable(echo_server ${SOURCES} src/socket_tests/echo_server.cpp)
add_executable(http_simple ${SOURCES} src/http_test/http_simple.cpp)
add_executable(http_simple2 ${SOURCES} src/http_test/http_simple2.cpp)
