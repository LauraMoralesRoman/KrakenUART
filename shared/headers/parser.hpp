#ifndef PARSER_HPP
#define PARSER_HPP

#include "callback_store.hpp"
#include "msg_ids.hpp"
#include "serial.hpp"
#include "utils.hpp"
#include "messages.hpp"
#include <cstdint>
#include <functional>
#include <utility>
#include <map>

namespace uahruart::parser {
    constexpr size_t BUFFER_SIZE = 64;

    class ParserState {
        public:
        template<typename T>
        ParserState(const T& func) : m_func(func) {}
        ParserState() = default;
        ParserState(ParserState&&) = default;
        ParserState& operator=(ParserState&& other);

        ParserState& operator=(const ParserState&) = delete;
        ParserState(const ParserState&) = delete;

        ParserState operator()();

        private:
        functor<ParserState()> m_func;
    };  

    class Protocol {
        public:
            Protocol();

            template<typename Callback_Args>
            void on_type(const IDs id, functor<void(const Callback_Args&)>&& callback) {
                m_store.on(id, std::move(callback));
            }

            void register_method(const char* device, const char* method, functor<messages::ActionFinished::Actions(int32_t)>&& callback);

            void receive(string& received);
            bool send(const serial::Serializable& serializable);

            bool call(const char* device, const char* method, int32_t arg);

            void on_write(functor<void(const char*)>&& callback);

        private:
            CallbackStore m_store;
            std::map<utils::hash_t, functor<messages::ActionFinished::Actions(int32_t)>> m_registered_methods;
            utils::hash_t m_call_uuid_seed = 123;

            ParserState s_idle();
            ParserState s_reading_header();
            ParserState s_reading_body();
            ParserState s_waiting_ack();

            messages::Header m_read_header;

            constexpr static int8_t SENDING_DATA = (1 << 0);
            constexpr static int8_t CAN_SEND = (1 << 1);
            constexpr static int8_t ERROR = (1 << 2);
            constexpr static int8_t PENDING_ACK = (1 << 3);

            // Parser states
            ParserState m_current_state;
            std::int8_t m_flags;

            serial::SerialBuffer m_buffer;
            std::array<char, BUFFER_SIZE> m_internal_buffer;

            functor<void(const char*)> m_on_write_callback;
            size_t m_sent_size = 0;
    };  

}

#endif // PARSER_HPP
