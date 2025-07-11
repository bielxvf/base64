#pragma once

#include <cstdint>
#include <ostream>

class BitStream {
public:
    BitStream(const std::size_t n = 32);
    ~BitStream();
    
    void append_bit(const uint8_t b);
    void append_n_bits(const uint64_t b, const std::size_t n);
    void append_byte(const uint8_t b);

    std::size_t size() const;
    uint32_t get_word(std::size_t i) const;
    uint8_t get_byte(std::size_t i) const;
    uint8_t get_bit(std::size_t i) const;

    friend std::ostream& operator<<(std::ostream& os, const BitStream& bs);
private:
    uint8_t* m_data;
    std::size_t m_count;
    std::size_t m_capacity;
};
