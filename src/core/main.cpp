#include <iostream>
#include <fstream>

#include <BitStream.hpp>

#include <cxxopts.hpp>

const char alphabet[64] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
    '+', '/'
};

int main(int argc, char** argv)
{

    cxxopts::Options options("base64", "Base64 encoding/decoding");
    options.add_options()
        ("d,decode", "Decode instead of encode")
        ("o,output", "Output file", cxxopts::value<std::string>())
        ("h,help", "Print help");
    /* TODO: Input flag, generalise the stream aswell like output */

    auto result = options.parse(argc, argv);
    bool decode = result.count("decode") > 0;

    std::string output_file;
    std::ostream* out = &std::cout;
    std::ofstream fout;

    if (result.count("output")) {
        output_file = result["output"].as<std::string>();
        fout.open(output_file);
        if (!fout) {
            std::cerr << "Error: Failed to open output file" << '\n';
            return 1;
        }
        out = &fout;
    }

    if (result.count("help")) {
        *out << options.help() << '\n';
        return 0;
    }

    if (decode) {
        std::unordered_map<char, uint8_t> decode_map;
        for (std::size_t i = 0; i < 64; i++) {
            decode_map[alphabet[i]] = i;
        }

        BitStream bs;
        for (char ch; std::cin.get(ch);) {
            if (ch != '=') {
                for (std::size_t i = 0; i < 6; i++) {
                    bs.append_bit((decode_map[ch] >> (5 - i)) & 1);
                }
            }
        }
        for (std::size_t i = 0; i < bs.size() / 8; i++) {
            *out << char(bs.get_byte(i));
        }

        return 0;

    } else {
        BitStream bs;
        for (char ch; std::cin.get(ch);) {
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
            *out << c;
        }

        return 0;
    }
}
