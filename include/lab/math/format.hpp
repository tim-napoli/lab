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

// SVG formater ---------------------------------------------------------------

void svg_header(std::ostream& os, float width, float height);

void svg_vec2(std::ostream& os, glm::vec2 v);

void svg_segment(std::ostream& os, segment s);

void svg_triangle(std::ostream& os, triangle t);

void svg_box(std::ostream& os, box b);

void svg_footer(std::ostream& os);

// ----------------------------------------------------------------------------

}}

#endif
