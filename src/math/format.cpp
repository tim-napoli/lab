#include "lab/math/format.hpp"

namespace lab { namespace math {

std::ostream& operator<<(std::ostream& os, glm::vec2 v) {
    os << "(" << v.x << ", " << v.y << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, segment s) {
    os << "(" << s.get_a() << ", " << s.get_b() << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, triangle t) {
    os << "(" << t.get_a() << ", " << t.get_b()
       << ", " << t.get_c() << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, box b) {
    os << "(" << b.bottom_left() << ", " << b.top_right()  << ")";
    return os;
}

}}

