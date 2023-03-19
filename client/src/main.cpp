#include <Arduino.h>
#include "protocol.hpp"

uahruart::CallbackStore store;

uahruart::parser::Protocol protocol;

uint32_t time_sleep = 100;
void setup() {
    Serial.begin(115200);
    store.on(uahruart::IDs::RPC_CALL, functor<void(uahruart::messages::RPCCall&)>{[&](auto& call) {
        time_sleep = call.arg.to_underlying();
    }});

    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    static uint32_t last_update = millis();
    if (millis() > (last_update + time_sleep)) {
        last_update = millis();
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }
    
    if (Serial.available()) {
        delayMicroseconds(100);
        String str = Serial.readString();
        uahruart::serial::SerialBuffer buffer(str.length(), &str[0]);
        uahruart::messages::Header tmp_header;
        auto after_header = buffer >> tmp_header >> uahruart::primitives::SEPARATOR;
        store.invoke(tmp_header.type.to_underlying(), after_header);
    }
}
