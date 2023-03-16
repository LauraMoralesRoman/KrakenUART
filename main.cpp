#include "shared/headers/callback_store.hpp"
#include "shared/headers/messages.hpp"
#include "shared/headers/msg_ids.hpp"
#include "shared/headers/primitives.hpp"
#include "shared/headers/utils.hpp"
#include "shared/protocol.hpp"

#include <iostream>

int main() {
    uahruart::CallbackStore store;
    store.on(uahruart::IDs::PRIMITIVE_INT, functor<void(const uahruart::primitives::Int&)>{[](auto& msg) {
        std::cout << "Entero: " << msg.to_underlying() << '\n';
    }});

    store.on(uahruart::IDs::TEST, functor<void(const uahruart::messages::Test&)>{[](auto& msg) {
                 std::cout << "Test: " << msg.number.to_underlying() << '\n';
    }});

     store.on(uahruart::IDs::RPC_CALL, functor<void (uahruart::messages::RPCCall&)>{[](auto& msg) {
      std::cout << "\nCall: " << msg.call_uuid.to_underlying() << ' ' << msg.arg.to_underlying() << ' ' << msg.function_hash.to_underlying() << '\n';
      }});

    char arr[64] = {};
    auto buffer = uahruart::serial::SerialBuffer(64, arr);

    uahruart::messages::RPCCall call;
    call.call_uuid = 1234567;
    call.arg = 500;
    call.function_hash = uahruart::utils::hash_string("traccion") ^ uahruart::utils::hash_string("desplazar");

    uahruart::messages::Header header;
    header.type = static_cast<uint32_t>(uahruart::IDs::RPC_CALL);
    header.length = 7654321;
    buffer << header << uahruart::primitives::SEPARATOR << call << uahruart::primitives::SEPARATOR;
    // buffer << test << uahruart::primitives::SEPARATOR;

    for (size_t i = 0; i < buffer.size(); i++)
        std::cout << buffer[i];
    std::cout << '\n';

    uahruart::serial::SerialBuffer read_buffer(64, arr);
    uahruart::messages::Header tmp_header;
    auto after_header = buffer >> tmp_header >> uahruart::primitives::SEPARATOR;
    std::cout << "\n\n";
    std::cout << tmp_header.length.to_underlying() << ' ' << tmp_header.type.to_underlying() << '\n';
    store.invoke(tmp_header.type.to_underlying(), after_header);
}
