
#ifndef IO_EVENT
#define IO_EVENT

#include <sys/types.h>
#include <deque>
#include <vector>
#include <functional>
#include <tcp/async_socket.h>

const size_t MAX_BUFFER_SIZE = 10000;
namespace tcp {
    struct read_buffer {
        char buf[MAX_BUFFER_SIZE];
        size_t needed;
        size_t done;
        bool read_all;
        std::function <void(int, async_socket*, size_t, void*)> call;
        read_buffer() = default;

        read_buffer(bool, size_t, std::function <void(int, async_socket*, size_t, void*)>);
    };

    struct write_buffer {
        char buf[MAX_BUFFER_SIZE];
        size_t needed;
        size_t done;
        std::function <void(int, async_socket*)> call;

        write_buffer() = default;

        write_buffer(void *, size_t, std::function <void(int, async_socket*)>);
    };

    struct connect_buffer {
        const char *ip;
        int port;
        std::function <void(int, async_socket*)> call;

        connect_buffer() = default;
        connect_buffer(const char *, int, std::function <void(int, async_socket*)>);
    };

    struct accept_buffer {
        async_socket* client;
        std::function <void(int, async_socket*)> call;

        accept_buffer() = default;
        accept_buffer(std::function <void(int, async_socket*)>);
    };

    struct io_events {
        io_events();
        io_events(async_socket*);
        io_events(int);
        ~io_events();

        size_t get_writers();
        size_t get_readers();

        void add_accept(accept_buffer);
        void add_connect(connect_buffer);
        void add_read(read_buffer);
        void add_write(write_buffer);

        bool want_accept();
        bool want_connect();
        bool want_read();
        bool want_write();

        bool run_accept();
        bool run_connect();
        bool run_read();
        bool run_write();

        void accept_call_back();
        void connect_call_back();
        void read_call_back();
        void write_call_back();

        void remove_client(async_socket*);

    private:

        int fd;
        async_socket* sock;
        int error = 0;
        std::deque <read_buffer> readers;
        std::deque <write_buffer> writers;
        std::deque <connect_buffer> connectors;
        std::deque <accept_buffer> acceptors;

        std::vector <async_socket*> clients;
    };

}

#endif