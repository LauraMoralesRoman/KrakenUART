#ifndef MSG_IDS_HPP
#define MSG_IDS_HPP

#include <cstdint>

namespace uahruart {
    enum class IDs : uint16_t {
        // Primitives
        PRIMITIVE_INT,
        PRIMITIVE_STR,
        PRIMITIVE_BOOL,
        // Messages
        TEST,
        ACK,
        RPC_CALL,
        RPC_RESPONSE,

        // Robot messages
        ODOMETRY,
        ACTION_FINISHED,

        // Do not modify
        SIZE,
        UNDEF
    };
}

#endif // MSG_IDS_HPP
