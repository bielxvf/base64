#include <iostream>
#include <vector>

#include "BitStream.hpp"

#include <cxxopts.hpp>

const char alphabet[64] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
    '+', '/'
};

bool decode = false;
std::string output_file;

int main(int argc, char** argv)
{
    BitStream bs;

    char ch;
    while (std::cin.get(ch)) {
        bs.append_byte(ch);
    }
    /* pad the input bytes to a multiple of 3, and remember how many actual bytes there were */
    std::size_t original_size = bs.size() / 8;
    for (std::size_t i = 0; i < (bs.size() / 8) % 3; i++) {
        bs.append_byte('\0');
    }

    std::size_t encoded_size = ((original_size + 2) / 3) * 4;
    std::vector<char> encoded(encoded_size);
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
        std::cout << c;
    }
}
