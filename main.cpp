#include "shared/headers/msg_ids.hpp"
#include "shared/headers/parser.hpp"
#include "shared/headers/primitives.hpp"
#include "shared/headers/utils.hpp"
#include <array>
#include <cstdint>
#include <iostream>

#include <functional>

int main() {
    uahruart::parser::Protocol protocol;

    protocol.register_method("traccion", "avanzar", [](int32_t arg) {
        std::cout << "Se ha avanzado " << arg << "mm\n";
        return uahruart::messages::ActionFinished::TRACTION;
    });

    protocol.register_method("traccion", "girar", [](int32_t arg) {
        std::cout << "Se han girado " << arg << "rad\n";
        return uahruart::messages::ActionFinished::TRACTION;
    });

    // std::array<char, uahruart::parser::BUFFER_SIZE> buffer;

    protocol.on_write([](const char* buffer) {
        std::cout << buffer << '\n';
    });

    // protocol.call("traccion", "avanzar");

    string msg = "4:-1156248511:1098118400:14:";
    protocol.receive(msg);

    // Should be on waiting ACK
    // string rcv = "1";
    // protocol.receive(rcv);
}
