#include "BitStream.hpp"

BitStream::BitStream(std::size_t n /* = 1 */)
{
    m_count = 0;
    m_capacity = n;
    m_data = new uint8_t[m_capacity]();
}

BitStream::~BitStream()
{
    delete[] m_data;
}
    
void BitStream::append_bit(uint8_t b)
{
    if (b > 1) return;

    if (m_count == m_capacity * 8) {
        std::size_t new_capacity = m_capacity * 2;
        uint8_t* new_data = new uint8_t[new_capacity](); 
        for (std::size_t i = 0; i < m_capacity; i++) {
            new_data[i] = m_data[i];
        }
        delete[] m_data;
        m_data = new_data;
        m_capacity = new_capacity;
    }

    std::size_t byte_index = m_count / 8;
    std::size_t bit_index = m_count % 8;

    if (b == 1) {
        m_data[byte_index] |= (1 << (7 - bit_index));
    } else {
        m_data[byte_index] &= ~(1 << (7 - bit_index));
    }

    m_count++;
}

void BitStream::append_byte(uint8_t b)
{
    if (m_count + 8 >= m_capacity * 8) {
        std::size_t new_capacity = m_capacity * 2;
        uint8_t* new_data = new uint8_t[new_capacity]; 
        for (std::size_t i = 0; i < m_capacity; i++) {
            new_data[i] = m_data[i];
        }
        delete[] m_data;
        m_data = new_data;
        m_capacity = new_capacity;
    }
    for (std::size_t i = 0; i < sizeof(b) * 8; i++) {
        BitStream::append_bit((b >> (7 - i)) & 1);
    }
}

std::size_t BitStream::size() const
{
    return m_count;
}

uint32_t BitStream::get_word(std::size_t i) const
{
    if ((i + 1) * 32 > m_count) {
        throw std::out_of_range("BitStream index out of range");
    }

    uint32_t result = 0;
    for (int b = 0; b < 32; ++b) {
        result <<= 1;
        std::size_t bit_index = i * 32 + b;
        std::size_t byte_index = bit_index / 8;
        std::size_t bit_in_byte = 7 - (bit_index % 8);
        uint8_t bit = (m_data[byte_index] >> bit_in_byte) & 1;
        result |= bit;
    }
    return result;
}

std::ostream& operator<<(std::ostream& os, const BitStream& bs)
{
    for (std::size_t i = 0; i < bs.m_count; i++) {
        std::size_t byte_index = i / 8;
        std::size_t bit_index = i % 8;
        uint8_t bit = (bs.m_data[byte_index] >> (7 - bit_index)) & 1;
        os << static_cast<int>(bit);
    }
    return os;
}
