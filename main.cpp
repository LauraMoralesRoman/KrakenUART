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

    protocol.register_method("traccion", "avanzar", [](int32_t arg) -> void{
        std::cout << "Se ha avanzado " << arg << "mm\n";
    });

    protocol.register_method("traccion", "girar", [](int32_t arg) -> void {
        std::cout << "Se han girado " << arg << "rad\n";
    });

    // std::array<char, uahruart::parser::BUFFER_SIZE> buffer;

    protocol.on_write([](const char* buffer, size_t size) {
        std::cout << buffer << '\n';
    });

    protocol.on_type(uahruart::IDs::PRIMITIVE_INT, std::function<void(const uahruart::primitives::Int&)>{[&](const auto& msg) {
        std::cout << "Entero: " << msg.to_underlying() << '\n';
    }});
    
    // protocol.call("traccion", "avanzar");

    string msg = "4:-1156248511:1098118400:14:";
    protocol.receive(msg);

    // Should be on waiting ACK
    // string rcv = "1";
    // protocol.receive(rcv);
}
