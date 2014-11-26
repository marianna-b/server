#include <exception>

#ifndef SIG_HANDLER_H
#define SIG_HANDLER_H

namespace tcp {
    template <typename T>
    struct async_type {
        async_type() = delete;
        async_type(bool f, T v, std::exception e) {
            correct = true;
            value = v;
            except = e;
        }

        T get() {
            if (correct)
                return value;
            throw except;
        }
    private:
        bool correct;
        T value;
        std::exception except;
    };
}

#endif // SIG_HANDLER_H