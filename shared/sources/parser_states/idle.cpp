#include "../../headers/parser.hpp"
#include <functional>

using namespace uahruart::parser;

#include <iostream>

ParserState Protocol::s_idle() {
    // Check if data was sent
    // if (m_flags & SENDING_DATA) {
    //     return std::bind(&Protocol::s_waiting_ack, this);
    // } else {
    //     return s_reading_header();
    // }
    // Read header

    messages::Header header;
    m_buffer = m_buffer >> header >> primitives::SEPARATOR;
        
    m_flags |= CAN_SEND;
    m_store.invoke(header.type.to_underlying(), m_buffer);
    m_flags &= ~CAN_SEND;

    // if (header.type.to_underlying() != static_cast<size_t>(IDs::ACK)) {
    //     serial::SerialBuffer internal_buffer(BUFFER_SIZE, m_internal_buffer.data());
    //     messages::Ack ack;
    //     messages::Header tmp_header;
    //     tmp_header.type = static_cast<size_t>(ack.type_id());
    //     internal_buffer << tmp_header << primitives::SEPARATOR << ack << primitives::SEPARATOR << '\0';
    //     m_on_write_callback(m_internal_buffer.data());
    // }

    return std::bind(&Protocol::s_idle, this);
}
