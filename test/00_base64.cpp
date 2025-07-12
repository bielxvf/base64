#include <base64.hpp>
#include <sstream>
#include <cassert>

int main() {
    std::istringstream in("hello");
    std::ostringstream out;
    base64::encode(in, out);

    std::string encoded = out.str();
    assert(encoded == "aGVsbG8=");

    std::istringstream in2(encoded);
    std::ostringstream out2;
    base64::decode(in2, out2);

    assert(out2.str() == "hello");
}
