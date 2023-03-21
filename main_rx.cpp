#include "shared/protocol.hpp"
#include "serial_port.hpp"

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
    std::cout << port << '\n';

    uahruart::parser::Protocol protocol;

    protocol.register_method("traccion", "avanzar", [](int32_t arg) -> void{
        std::cout << "Se ha avanzado " << arg << "mm\n";
    });

    protocol.register_method("traccion", "girar", [](int32_t arg) -> void {
        std::cout << "Se han girado " << arg << "rad\n";
    });

    // std::array<char, uahruart::parser::BUFFER_SIZE> buffer;

    protocol.on_write([port](const char* buff) {
        std::cout << "Writing bytes: " << buff << '\n';
        size_t ammount = strlen(buff);
        write(port, buff, ammount);
        write(port, "\n", 1); // Flush 
    });
    
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
