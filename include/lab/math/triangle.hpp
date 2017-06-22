#ifndef _lab_math_triangle_hpp_
#define _lab_math_triangle_hpp_

#include <glm/glm.hpp>

namespace lab { namespace math {

class triangle {
  private:
    glm::vec2 _a, _b, _c;

  public:
    triangle(glm::vec2 a, glm::vec2 b, glm::vec2 c);

    ~triangle();

    glm::vec2 get_a() const {return _a;}
    glm::vec2 get_b() const {return _b;}
    glm::vec2 get_c() const {return _c;}

    /**
     * @return the triangle area.
     */
    float area() const;

    /**
     * @return true if the given point is in the triangle.
     */
    bool contains(glm::vec2 p) const;
};

}}

#endif

