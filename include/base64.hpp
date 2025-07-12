#pragma once

#include <iostream>

namespace base64 {
    void encode(std::istream& in, std::ostream& out);
    void decode(std::istream& in, std::ostream& out);
}
