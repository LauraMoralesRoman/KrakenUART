#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include "msg_ids.hpp"
#include "primitives.hpp"
#include "serial.hpp"
#include <cstdint>

namespace uahruart::messages {
    /**
     * @brief This enum contains all the available protocol messages implemented. 
     */
    enum Types : uint8_t {
        // Protocol messages
        TEST,
        ACK,
        // RMI messages
        RPC_CALL,
        RPC_RESPONSE,

        // Custom messages
        ODOMETRY,
        ACTION_FINISHED
    };

    // ==================================================
    // Protocol messages
    // ==================================================
    
    /**
     * @brief Protocol message base class containing all methods neccessary to send a message
     * @details
     * Sending this kind of message allows the protocol to identify the message types. This is done by adding a header containing
     * message type information that is processed automatically.
     */
    class ProtocolMessage : virtual public serial::Serializable {
        public:
            /**
             * @brief This method provides the type value of the class. Return an enum element of type Type.
             * @details 
             * Every class has to have a message type that allows identification in the other end of the communication.
             */
            virtual uint8_t type() const = 0;
    };

    /**
     * @brief Header of every message it contains the length of the message and a checksum of both the header and the entire message
     * @details The checksum calculation is performed in the following order:
     */
    class Header : virtual public serial::Serializable {
        public:
            serial::SerialBuffer serialize(serial::SerialBuffer& buffer) const override;
            serial::SerialBuffer deserialize(const serial::SerialBuffer& buffer) override;
            
            virtual const IDs type_id() const override {return IDs::UNDEF;}

            primitives::Int type = static_cast<uint16_t>(IDs::UNDEF);
    };


    // ==================================================
    // Protocol messages
    // ==================================================
    class Test : virtual public ProtocolMessage {
        public:
            primitives::Int number;

            serial::SerialBuffer serialize(serial::SerialBuffer& buffer) const override;
            serial::SerialBuffer deserialize(const serial::SerialBuffer& buffer) override;

            uint8_t type() const override {return TEST;}
            virtual const IDs type_id() const override {return IDs::TEST;}
    };

    class Ack : virtual public ProtocolMessage {
        public:
            serial::SerialBuffer serialize(serial::SerialBuffer& buffer) const override;
            serial::SerialBuffer deserialize(const serial::SerialBuffer& buffer) override;

            uint8_t type() const override {return ACK;}
            virtual const IDs type_id() const override {return IDs::ACK;}
    };

    // ==================================================
    // RMI messages
    // ==================================================
    class RPCCall : virtual public ProtocolMessage {
        public:
            primitives::Int function_hash;
            primitives::Int call_uuid;
            primitives::Int arg;

            serial::SerialBuffer serialize(serial::SerialBuffer& buffer) const override;
            serial::SerialBuffer deserialize(const serial::SerialBuffer& buffer) override;

            uint8_t type() const override {return RPC_CALL;}
            virtual const IDs type_id() const override {return IDs::RPC_CALL;}
    };

    class RPCResponse : virtual public ProtocolMessage {
        public:
            primitives::Int call_uuid;
            primitives::Int ret;
            primitives::Bool valid = true;

            serial::SerialBuffer serialize(serial::SerialBuffer& buffer) const override;
            serial::SerialBuffer deserialize(const serial::SerialBuffer& buffer) override;

            uint8_t type() const override {return RPC_RESPONSE;}
            virtual const IDs type_id() const override {return IDs::RPC_RESPONSE;}
    };

    // ==================================================
    // Custom messages
    // ==================================================
    class Odometry : virtual public ProtocolMessage {
        public:
            primitives::Int x;
            primitives::Int y;
            primitives::Int o;

            serial::SerialBuffer serialize(serial::SerialBuffer& buffer) const override;
            serial::SerialBuffer deserialize(const serial::SerialBuffer& buffer) override;

            uint8_t type() const override {return ODOMETRY;}
            virtual const IDs type_id() const override {return IDs::ODOMETRY;}
    };

    class ActionFinished : virtual public ProtocolMessage {
        public:
            primitives::Int action;

            serial::SerialBuffer serialize(serial::SerialBuffer& buffer) const override;
            serial::SerialBuffer deserialize(const serial::SerialBuffer& buffer) override;

            // Finished actions
            enum Actions {
                TRACTION,
                SERVO,
                // Keep this ones you dumb fuck
                SIZE,
                NONE // If that action doesn't finish (or does so immediately)
            };

            uint8_t type() const override {return ACTION_FINISHED;}
            virtual const IDs type_id() const override {return IDs::ACTION_FINISHED;}
    };
}

#endif // MESSAGES_HPP
