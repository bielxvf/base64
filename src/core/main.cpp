#include <iostream>
#include <fstream>
#include <cxxopts.hpp>
#include <base64.hpp>

int main(int argc, char** argv)
{
    cxxopts::Options options("base64", "Base64 encoding/decoding");
    options.add_options()
        ("d,decode", "Decode instead of encode")
        ("o,output", "Output file", cxxopts::value<std::string>())
        ("i,input", "Input file", cxxopts::value<std::string>())
        ("h,help", "Print help");

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

    std::string input_file;
    std::istream* in = &std::cin;
    std::ifstream fin;
    if (result.count("input")) {
        input_file = result["input"].as<std::string>();
        fin.open(input_file);
        if (!fin) {
            std::cerr << "Error: Failed to open input file" << '\n';
            return 1;
        }
    }

    if (result.count("help")) {
        *out << options.help() << '\n';
        return 0;
    }

    if (decode) {
        base64::decode(*in, *out);
    } else {
        base64::encode(*in, *out);
    }
}
