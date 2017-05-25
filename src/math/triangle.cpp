#include "lab/math/common.hpp"
#include "lab/math/triangle.hpp"

namespace lab { namespace math {

triangle::triangle(glm::vec2 a, glm::vec2 b, glm::vec2 c)
        : _a(a), _b(b), _c(c)
{

}

triangle::~triangle() {

}

float triangle::area() const {
    return std::abs(determinant(_b - _a, _b - _c)) / 2;
}

bool triangle::contains(glm::vec2 p) const {
    triangle A = triangle(_a, _b, p),
             B = triangle(_a, _c, p),
             C = triangle(_b, _c, p);
    return float_are_equal(area(), A.area() + B.area() + C.area());
}

}}

