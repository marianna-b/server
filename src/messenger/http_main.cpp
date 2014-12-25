#include "messenger/messenger.h"
#include <iostream>

int main() {
    try {
        messenger m;
        m.start();
        return 0;
    }
    catch (std::exception const& e)
    {
        std::cout << "error: " << e.what() << std::endl;
        return 1;
    }
}