#include "test/math/common.hpp"

namespace lab { namespace test {

float gen_float(float a, float b) {
    int mod_val = (b - a) * 10000 + 1;
    return ((rand() % mod_val) / 10000.0) + a;
}

glm::vec2 gen_vec2(float xmin, float xmax, float ymin, float ymax) {
    return glm::vec2(gen_float(xmin, xmax), gen_float(ymin, ymax));
}

}}

std::ostream& operator<<(std::ostream& stream, glm::vec2 v) {
    stream << "(" << v.x << ", " << v.y << ")";
    return stream;
}

