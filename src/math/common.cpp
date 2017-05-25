#include "lab/math/common.hpp"

namespace lab { namespace math {

const float epsilon = 1E-3;

bool float_are_equal(float a, float b) {
    return std::abs(b - a) < epsilon;
}

bool vec2_are_equal(glm::vec2 a, glm::vec2 b) {
    return float_are_equal(a.x, b.x) && float_are_equal(a.y, b.y);
}

}}

