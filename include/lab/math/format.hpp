/**
 * @module      lab::math::format
 * @brief       Formaters for different lab math objects.
 * @author      Timothée Napoli
 * @license     WTFPL
 * @copyright   Timothée Napoli (c) 2017
 */
#ifndef _lab_math_format_hpp_
#define _lab_math_format_hpp_

#include <iostream>
#include <glm/glm.hpp>
#include "lab/math/segment.hpp"
#include "lab/math/triangle.hpp"
#include "lab/math/box.hpp"

namespace lab { namespace math {

std::ostream& operator<<(std::ostream& os, glm::vec2 v);

std::ostream& operator<<(std::ostream& os, segment s);

std::ostream& operator<<(std::ostream& os, triangle t);

std::ostream& operator<<(std::ostream& os, box b);

}}

#endif
