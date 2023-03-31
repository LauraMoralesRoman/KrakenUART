#include "shared/protocol.hpp"
#include "serial_port.hpp"

#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <thread>
#include <cstring>
#include <list>

int main (int argc, char *argv[]) {
    std::cout << "Opening server\n";
    if (argc != 2) {
        std::cout << "Not enough arguments (pass the modem)\n";
        return 1;
    }

    auto port = open_serial(argv[1]);

    if (port == -1) {
        std::cout << "Could not open port " << argv[1] << '\n';
        return 1;
    }

    uahruart::parser::Protocol protocol;


    protocol.on_write([port](const char* buff) {
        std::cout << "Writing bytes: " << buff << '\n';
        size_t ammount = strlen(buff);
        write(port, buff, ammount);
        write(port, "\n\0", 2); // Flush 
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

    std::list<std::pair<std::string, int32_t>> commands {
        {"advance", 500},
        {"advance", -500},
        {"advance", 500},
        {"advance", -500},
        {"advance", 500},
        {"advance", -500},
        // {"turn", 90}
    };

    protocol.on_type(uahruart::IDs::PRIMITIVE_BOOL, functor<void (const uahruart::primitives::Bool&)>{[&](auto& msg) {
        std::cout << "Sending next waypoint\n";
        auto current = commands.front();
        commands.pop_front();
        protocol.call("traction", current.first.c_str(), current.second);
    }});
    
    auto current = commands.front();
    commands.pop_front();
    protocol.call("traction", current.first.c_str(), current.second);


    read_thread.join();
}
