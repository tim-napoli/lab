#ifndef _lab_math_segment_hpp_
#define _lab_math_segment_hpp_

#include <glm/glm.hpp>
#include "lab/math/common.hpp"
#include "lab/util/optional.hpp"

namespace lab { namespace math {

class segment {
  private:
    glm::vec2 _a, _b;

    util::optional<glm::vec2>
    get_intersection_colinears(const segment& other) const;

    util::optional<glm::vec2>
    get_intersection_crossing(const segment& other) const;

  public:
    segment(glm::vec2 start, glm::vec2 stop);

    ~segment();

    glm::vec2 get_direction() const {return _b - _a;}

    util::optional<glm::vec2> get_intersection(const segment& other) const;
};

}}

#endif
