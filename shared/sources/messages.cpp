#include "../headers/messages.hpp"

using namespace uahruart::messages;

#include <iostream>

uahruart::serial::SerialBuffer Header::serialize(serial::SerialBuffer &buffer) const {
    return buffer << type;
}

uahruart::serial::SerialBuffer Header::deserialize(const serial::SerialBuffer &buffer) {
    return buffer >> type;
}

uahruart::serial::SerialBuffer Test::serialize(serial::SerialBuffer &buffer) const {
    return buffer << number;
}

uahruart::serial::SerialBuffer Test::deserialize(const serial::SerialBuffer &buffer) {
    return buffer >> number;
}

uahruart::serial::SerialBuffer RPCCall::serialize(serial::SerialBuffer &buffer) const {
    return buffer << function_hash << primitives::SEPARATOR << call_uuid << primitives::SEPARATOR << arg;
}

uahruart::serial::SerialBuffer RPCCall::deserialize(const serial::SerialBuffer &buffer) {
    return buffer >> function_hash >> primitives::SEPARATOR >> call_uuid >> primitives::SEPARATOR >> arg;
}

uahruart::serial::SerialBuffer RPCResponse::serialize(serial::SerialBuffer &buffer) const {
    return buffer << call_uuid << primitives::SEPARATOR << ret << primitives::SEPARATOR << valid;
}

uahruart::serial::SerialBuffer RPCResponse::deserialize(const serial::SerialBuffer &buffer) {
    return buffer >> call_uuid >> primitives::SEPARATOR >> ret >> primitives::SEPARATOR >> valid;
}

uahruart::serial::SerialBuffer Odometry::serialize(serial::SerialBuffer &buffer) const {
    return buffer << x << primitives::SEPARATOR << y << primitives::SEPARATOR << o;
}

uahruart::serial::SerialBuffer Odometry::deserialize(const serial::SerialBuffer &buffer) {
    return buffer >> x >> primitives::SEPARATOR >> y >> primitives::SEPARATOR >> o;
}

uahruart::serial::SerialBuffer ActionFinished::serialize(serial::SerialBuffer &buffer) const {
    return buffer << action;
}

uahruart::serial::SerialBuffer ActionFinished::deserialize(const serial::SerialBuffer &buffer) {
    return buffer >> action;
}
