#include <iostream>
#include "lab/math/common.hpp"
#include "lab/math/segment.hpp"

namespace lab { namespace math {

segment::segment(glm::vec2 a, glm::vec2 b)
        : _a(a)
        , _b(b)
{

}

segment::~segment() {

}

float determinant(glm::vec2 a, glm::vec2 b) {
    return a.x * b.y - b.x * a.y;
}

bool is_in_range(float x, float a, float b) {
    return float_are_equal(std::abs(b - a), std::abs(a - x) + std::abs(b - x));
}

util::optional<glm::vec2>
segment::get_intersection_colinears(const segment& other) const
{
    // Here, we need to know if the two vectors are on the same line.
    // To do so, we can check the area of the four points. If this area
    // is zero, then vectors are on the same line, and we can go further,
    // otherwise, no intersection is possible.
    glm::vec2 a = get_direction();
    glm::vec2 b = other._a - _a;
    float vecp = determinant(a, b);
    if (!float_are_equal(vecp, 0.0)) {
        return util::optional<glm::vec2>::none();
    }

    // Once we know that segments are on the same line, we can project them on
    // the x-axis
    float px1 = _a.x, px2 = _b.x;
    float qx1 = other._a.x, qx2 = other._b.x;

    // TODO return the intersecting segment (for now, we only send the
    //      "middle" point).
    if (is_in_range(qx1, px1, px2)) {
        return util::optional<glm::vec2>::value(other._a);
    } else
    if (is_in_range(qx2, px1, px2)) {
        return util::optional<glm::vec2>::value(other._b);
    } else
    if (is_in_range(px1, qx1, qx2)) {
        return util::optional<glm::vec2>::value(_a);
    }
    if (is_in_range(px2, qx1, qx2)) {
        return util::optional<glm::vec2>::value(_b);
    }

    return util::optional<glm::vec2>::none();
}

util::optional<glm::vec2>
segment::get_intersection_crossing(const segment& other) const
{
    glm::vec2 p = get_a();
    glm::vec2 q = other.get_a();
    glm::vec2 r = get_direction();
    glm::vec2 s = other.get_direction();

    float t = determinant(q - p, s) / determinant(r, s);
    float u = determinant(q - p, r) / determinant(r, s);

    if (is_in_range(t, 0.0, 1.0) && is_in_range(u, 0.0, 1.0)) {
        return util::optional<glm::vec2>::value(p + t * r);
    }

    return util::optional<glm::vec2>::none();
}

util::optional<glm::vec2> segment::get_intersection(const segment& other) const {
    glm::vec2 r = get_direction();
    glm::vec2 s = other.get_direction();

    // First we check if vectors are colinears or not.
    float vecp = determinant(r, s);
    if (float_are_equal(vecp, 0.0)) {
        return get_intersection_colinears(other);
    }
    return get_intersection_crossing(other);
}

}}

