#include <iostream>

#include "BitStream.hpp"

char alphabet[64] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
    '+', '/'
};

int main(int argc, char** argv)
{
    BitStream bs;

    char ch;
    while (std::cin.get(ch)) {
        bs.append_byte(ch);
    }
    /* How could I pad the bitstream before encoding it? */
    /* Should input be multiple of 24 right? */
    /* what if we add 0's and just save the "old length" and when we reach past that old length we simply print '='?*/

    /* Group them in four 6-bit groups */
    for (std::size_t i = 0; i < bs.size(); i += 24) {
        for (std::size_t j = 0; j < 24; j += 6) {
            uint8_t value = 0;
            for (std::size_t k = 0; k < 6; k++) {
                value <<= 1;
                value |= bs.get_bit(j + k);
            }
            std::cout << alphabet[value];
        }
    }
}
