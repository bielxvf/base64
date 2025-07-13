#include <base64.hpp>
#include <sstream>
#include <cassert>

int main() {
    std::istringstream in("M");
    std::ostringstream out;
    base64::encode(in, out);
    std::string encoded = out.str();
    assert(encoded == "TQ==");

    std::istringstream in2("Ma");
    std::ostringstream out2;
    base64::encode(in2, out2);
    std::string encoded2 = out2.str();
    assert(encoded2 == "TWE=");

    std::istringstream in3("Man");
    std::ostringstream out3;
    base64::encode(in3, out3);
    std::string encoded3 = out3.str();
    assert(encoded3 == "TWFu");

    std::istringstream in4("");
    std::ostringstream out4;
    base64::encode(in4, out4);
    std::string encoded4 = out4.str();
    assert(encoded4 == "");
}
