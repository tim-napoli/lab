#ifndef _lab_test_math_common_hpp_
#define _lab_test_math_common_hpp_

#include <iostream>
#include <glm/glm.hpp>

namespace lab { namespace test {

float gen_float(float a, float b);

glm::vec2 gen_vec2(float xmin, float xmax, float ymin, float ymax);

}}

std::ostream& operator<<(std::ostream& stream, glm::vec2 v);

#endif

