#ifndef PARSER_HPP
#define PARSER_HPP

#include "callback_store.hpp"
#include "msg_ids.hpp"
#include "serial.hpp"
#include "utils.hpp"
#include <cstdint>

namespace uahruart::parser {
    struct ParserContext {
        serial::SerialBuffer buffer;
    };  
    
    class ParserState;
    class Protocol {
        public:
            template<typename Callback_Arg>
            void on_type(const IDs id, functor<const Callback_Arg&> callback);

            void register_method(const char* device, const char* method, const functor<int32_t(int32_t)>& callback);


        private:
        CallbackStore m_store;

        // Parser states
        ParserState* m_current_state = nullptr;
    };  

    class ParserState {
        public:
        virtual ParserState* parse(ParserContext& ctx) = 0;
        virtual ParserState* on_enter(ParserContext& ctx) = 0;
        private:
    };

    class ReadingHeader : virtual public ParserState {
        public:
            virtual ParserState* parse(ParserContext& ctx) override;
            virtual ParserState* on_enter(ParserContext& ctx) override;
    };

    class ReadingBody : virtual public ParserState {
        public:
            virtual ParserState* parse(ParserContext& ctx) override;
            virtual ParserState* on_enter(ParserContext& ctx) override;
    };

    class WaitingACK : virtual public ParserState {
        public:
            virtual ParserState* parse(ParserContext& ctx) override;
            virtual ParserState* on_enter(ParserContext& ctx) override;
    };

    
}

#endif // PARSER_HPP
