#include "shared/protocol.hpp"
#include "serial_port.hpp"

#include <cstdio>
#include <cstdlib>
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

    if (port < 0) {
        std::cout << "Could not open port " << argv[1] << '\n';
        exit(1);
    }

    uahruart::parser::Protocol protocol;

    protocol.on_write([port](const char* buff) {
        std::cout << "Writing bytes: " << buff << '\n';
        size_t ammount = strlen(buff);
        std::cout << "Strlen: " << ammount << '\n';
        write(port, buff, ammount);
        write(port, "\0", 1); // Flush 
    });

    protocol.on_type(uahruart::IDs::PRIMITIVE_INT, functor<void (const uahruart::primitives::Int&)>{[&](auto msg) {
        std::cout << "Received integer: " << msg.to_underlying() << '\n';
    }});

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
	std::string cmd;
        int arg;
        std::cin >> cmd >> arg;
        protocol.call("traccion", cmd.c_str(), arg);
    }

    read_thread.join();
}
