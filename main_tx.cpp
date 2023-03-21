#include "shared/protocol.hpp"
#include "serial_port.hpp"

#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <thread>
#include <cstring>

int main (int argc, char *argv[]) {
    std::cout << "Opening TX server\n";
    if (argc != 2) {
        std::cout << "Not enough arguments (pass the modem)\n";
        return 1;
    }

    auto port = open_serial(argv[1]);

    uahruart::parser::Protocol protocol;

    protocol.on_write([port](const char* buff) {
        std::cout << "Writing bytes: " << buff << '\n';
        size_t ammount = strlen(buff);
        write(port, buff, ammount);
        write(port, "\n", 1); // Flush 
    });

    auto read_thread = std::thread([&]() {
        string str;
        while (true) {
            char c;
            read(port, &c, 1);
            std::cout << c << '\n';
            if (c == '\0' || c == '\n') {
                protocol.receive(str);
                str = "";
            } else {
                str += c;

            }
        }
    });

    while (true) {
        std::cout << "Call: ";
        int arg;
        std::cin >> arg;
        protocol.call("traccion", "avanzar", arg);
    }

    read_thread.join();
}
