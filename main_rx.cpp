#include "shared/protocol.hpp"
#include "serial_port.hpp"

#include <functional>
#include <iostream>
#include <unistd.h>
#include <cstring>

int main (int argc, char *argv[])
{
    std::cout << "Opening RX server\n";
    if (argc != 2) {
        std::cout << "Not enough arguments (pass the modem)\n";
        return 1;
    }

    // Open serial port
    auto port = open_serial(argv[1]);

    if (port < 0) {
        std::cout << "Could not open port " << argv[1] << '\n';
        exit(1);
    }

    uahruart::parser::Protocol protocol;

    protocol.register_method("traccion", "avanzar", [&](int32_t arg) {
        std::cout << "Se ha avanzado " << arg << "mm\n";
        std::cout.flush();
        // uahruart::messages::ActionFinished finished;
        // finished.action = arg;
        // protocol.send(finished);
        return uahruart::messages::ActionFinished::TRACTION;
    });

    protocol.register_method("traccion", "girar", [&](int32_t arg) {
        std::cout << "Se han girado " << arg << "rad\n";
        std::cout.flush();
        uahruart::messages::Odometry odom;
        odom.x = 123;
        odom.y = 456;
        odom.o = 789;
        protocol.send(odom);
        return uahruart::messages::ActionFinished::TRACTION;
    });

    protocol.register_method("admin", "test", [&](int32_t arg) {
        std::cout << "Sending ACK\n";
        std::cout.flush();
        uahruart::messages::Ack ack;
        protocol.send(ack);
        return uahruart::messages::ActionFinished::NONE;
    });

    // std::array<char, uahruart::parser::BUFFER_SIZE> buffer;

    protocol.on_write([port](const char* buff) {
        size_t ammount = strlen(buff);
        write(port, buff, ammount);
        write(port, "\0", 1); // Flush 
    });

    protocol.on_type(uahruart::IDs::ACTION_FINISHED, std::function<void(const uahruart::messages::ActionFinished&)>{[](auto& msg) {
        std::cout << msg.action.to_underlying() << '\n';
    }});
    
    string str;
    while (true) {
        char c;
        read(port, &c, 1);
        std::cout << c;
        if (c == '\0' || c == '\n') {
            protocol.receive(str);
            str = "";
        } else {
            str += c;
        }
    }


    return 0;
}   
