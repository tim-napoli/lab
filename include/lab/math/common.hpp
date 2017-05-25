#ifndef _lab_math_common_hpp_
#define _lab_math_common_hpp_

#include <glm/glm.hpp>

namespace lab { namespace math {

bool float_are_equal(float a, float b);

bool vec2_are_equal(glm::vec2 a, glm::vec2 b);

float determinant(glm::vec2 a, glm::vec2 b);

/**
 * Check if x is in [a, b].
 */
bool is_in_range(float x, float a, float b);

}}

#endif

