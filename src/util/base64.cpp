#include <base64.hpp>
#include <unordered_map>
#include <BitStream.hpp>

namespace {

const char alphabet[64] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
    '+', '/'
};

}

namespace base64 {

void encode(std::istream& in, std::ostream& out)
{
    BitStream bs;
    for (char ch; in.get(ch);) {
        bs.append_byte(ch);
    }

    /* pad the input bytes to a multiple of 3, and remember how many actual bytes there were */
    std::size_t original_size = bs.size() / 8;
    for (std::size_t i = 0; i < (bs.size() / 8) % 3; i++) {
        bs.append_byte('\0');
    }

    std::size_t encoded_size = ((original_size + 2) / 3) * 4;
    std::string encoded(encoded_size, '\0');
    std::size_t l = 0;
    for (std::size_t i = 0; i < bs.size(); i += 24) {
        for (std::size_t j = 0; j < 24; j += 6) {
            uint8_t value = 0;
            for (std::size_t k = 0; k < 6; k++) {
                value <<= 1;
                value |= bs.get_bit(i + j + k);
            }
            encoded[l++] = alphabet[value];
        }
    }

    std::size_t padding_size = bs.size() / 8 - original_size;
    for (std::size_t i = 0; i < padding_size; i++) {
        encoded[encoded_size - 1 - i] = '=';
    }

    for (auto c : encoded) {
        out << c;
    }
}

void decode(std::istream& in, std::ostream& out)
{
    std::unordered_map<char, uint8_t> decode_map;
    for (std::size_t i = 0; i < 64; i++) {
        decode_map[alphabet[i]] = i;
    }

    BitStream bs;
    for (char ch; in.get(ch);) {
        if (ch != '=') {
            for (std::size_t i = 0; i < 6; i++) {
                bs.append_bit((decode_map[ch] >> (5 - i)) & 1);
            }
        }
    }
    for (std::size_t i = 0; i < bs.size() / 8; i++) {
        out << char(bs.get_byte(i));
    }
}

}
