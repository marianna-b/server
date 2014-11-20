cmake_minimum_required(VERSION 2.8.4)
project(server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

include_directories(src)
set(SOURCE_FILES
                    src/tcp/socket.h
                    src/tcp/socket.cpp
                    src/tcp/server.h
                    src/tcp/server.cpp
                    src/tcp/client.h
                    src/tcp/client.cpp
                    src/tcp/epoll.h
                    src/tcp/epoll.cpp
                    src/main.cpp)

add_executable(server ${SOURCE_FILES})