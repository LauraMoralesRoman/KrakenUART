#include "../headers/serial.hpp"

using namespace uahruart::serial;

#include <iostream>

SerialBuffer uahruart::serial::operator<<(SerialBuffer buffer, const Serializable& serializable) {
    auto new_buffer = serializable.serialize(buffer);
    // new_buffer.m_ammount_rw += ammount;

    // SerialBuffer::on_serialize(ammount, new_buffer);

    return new_buffer;
}

SerialBuffer uahruart::serial::operator>>(const SerialBuffer buffer, Serializable& serializable) {
    auto new_buffer = serializable.deserialize(buffer);
    // new_buffer.m_ammount_rw += ammount;
    // SerialBuffer::on_serialize(ammount, new_buffer);

    return new_buffer;
}

SerialBuffer::SerialBuffer(const size_t size, char* buffer) 
    : m_SIZE(size), m_buffer(buffer), m_buffer_start(0) {
    
}

SerialBuffer::SerialBuffer(const size_t size, char* buffer, const size_t start)
    : m_SIZE(size), m_buffer(buffer), m_buffer_start(start) {

}

SerialBuffer::SerialBuffer()
    : m_SIZE(1), m_buffer(nullptr), m_buffer_start(0) {

}

SerialBuffer SerialBuffer::operator+(const size_t ammount) const {
    return {m_SIZE, m_buffer, (m_buffer_start + ammount) % m_SIZE};
}

SerialBuffer& SerialBuffer::operator+=(const size_t ammount) {
    m_buffer_start = (m_buffer_start + ammount) % m_SIZE;
    return *this;
}

char SerialBuffer::operator[](const size_t index) const {
    return m_buffer[(index + m_buffer_start) % m_SIZE];
}

char& SerialBuffer::operator[](const size_t index) {
    return m_buffer[(index + m_buffer_start) % m_SIZE];
}

SerialBuffer::operator char *() const {
    return m_buffer + m_buffer_start;
}
