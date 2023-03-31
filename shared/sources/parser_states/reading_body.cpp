#include "../../headers/parser.hpp"

using namespace uahruart::parser;

ParserState Protocol::s_reading_body() {
    // Deserialize depending on type_id
    m_on_write_callback("1:"); // Send ACK

    m_flags |= CAN_SEND;
    m_store.invoke(m_read_header.type.to_underlying(), m_buffer);
    m_flags &= ~CAN_SEND;

    if (m_flags & SENDING_DATA)
        return std::bind(&Protocol::s_waiting_ack, this);
    else
        return std::bind(&Protocol::s_idle, this);
}
