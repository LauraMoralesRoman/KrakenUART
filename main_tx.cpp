#include "shared/protocol.hpp"
#include "serial_port.hpp"

#include <iostream>

int main (int argc, char *argv[]) {
    std::cout << "Opening TX server\n";
    if (argc != 2) {
        std::cout << "Not enough arguments (pass the modem)\n";
        return 1;
    }

    auto port = open_serial(argv[1]);
    std::cout << port << '\n';
}
