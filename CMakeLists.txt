cmake_minimum_required(VERSION 2.8.4)
project(tcp_server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

include_directories(src)
set(SOURCE_ECHO_CLIENT
                    src/tcp/async_socket.h
                    src/tcp/async_socket.cpp
                    src/tcp/async_server.h
                    src/tcp/async_server.cpp
                    src/async_service/epoll.h
                    src/async_service/epoll.cpp
                    src/async_service/io_service.h
                    src/async_service/io_service.cpp
                    src/echo_client.cpp)

set(SOURCE_ECHO_SERVER
                    src/tcp/async_socket.h
                    src/tcp/async_socket.cpp
                    src/tcp/async_server.h
                    src/tcp/async_server.cpp
                    src/async_service/epoll.h
                    src/async_service/epoll.cpp
                    src/async_service/io_service.h
                    src/async_service/io_service.cpp
                    src/echo_server.cpp)



set(SOURCE_ASYNC_TEST
                    src/tcp/async_socket.h
                    src/tcp/async_socket.cpp
                    src/tcp/async_server.h
                    src/tcp/async_server.cpp
                    src/async_service/epoll.h
                    src/async_service/epoll.cpp
                    src/async_service/io_service.h
                    src/async_service/io_service.cpp
                    src/async_test.cpp)


add_executable(async_test ${SOURCE_ASYNC_TEST})
add_executable(echo_client ${SOURCE_ECHO_CLIENT})
add_executable(echo_server ${SOURCE_ECHO_SERVER})
