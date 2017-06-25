#include "lab/math/format.hpp"

using namespace lab::math;

int main(void) {
    svg_header(std::cout, 1.0, 1.0);
    svg_vec2(std::cout, glm::vec2(0.5, 0.5));
    svg_segment(std::cout, segment(glm::vec2(0.3, 0.3), glm::vec2(0.7, 0.1)));
    svg_triangle(std::cout,
        triangle(glm::vec2(0.1, 0.1), glm::vec2(0.1, 0.5), glm::vec2(0.7, 0.3))
    );
    svg_box(std::cout, box(glm::vec2(0.6, 0.4), glm::vec2(0.9, 0.7)));
    svg_footer(std::cout);

    return 0;
}

