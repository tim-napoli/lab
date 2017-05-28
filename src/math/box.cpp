#include <cassert>
#include "lab/math/box.hpp"

namespace lab { namespace math {

box::box(glm::vec2 bottom_left, glm::vec2 top_right)
    : _bottom_left(bottom_left)
    , _top_right(top_right)
{
    assert (bottom_left.x <= top_right.x);
    assert (bottom_left.y <= top_right.y);
}

box::box(float left, float right, float bottom, float top)
    : _bottom_left(left, bottom)
    , _top_right(right, top)
{
    assert (left <= right);
    assert (bottom <= top);
}

box::~box() {

}

bool box::contains(glm::vec2 p) const {
    return !(   p.x < _bottom_left.x
             || p.x > _top_right.x
             || p.y < _bottom_left.y
             || p.y > _top_right.y
            );
}

bool box::intersects(const box& other) const {
    return !(   other.right() < left()
             || other.left() > right()
             || other.top() < bottom()
             || other.bottom() > top()
            );
}

}}

