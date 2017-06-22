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
    segment();
    segment(glm::vec2 start, glm::vec2 stop);

    ~segment();

    glm::vec2 get_a() const {return _a;}
    glm::vec2 get_b() const {return _b;}

    void set_a(glm::vec2 a) {_a = a;}
    void set_b(glm::vec2 b) {_b = b;}

    glm::vec2 get_direction() const {return _b - _a;}

    util::optional<glm::vec2> get_intersection(const segment& other) const;
};

}}

#endif
