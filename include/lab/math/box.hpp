/**
 * @module      lab::math::box
 * @brief       The box math class.
 * @author      Timothée Napoli
 * @license     WTFPL
 * @copyright   Timothée Napoli (c) 2017
 */
#ifndef _lab_math_box_hpp_
#define _lab_math_box_hpp_

#include <glm/glm.hpp>

namespace lab { namespace math {

class box {
  private:
    glm::vec2 _bottom_left;
    glm::vec2 _top_right;

  public:
    /**
     * @pre bottom_left.x <= top_right.x
     * @pre bottom_left.y <= top_right.y
     */
    box(glm::vec2 bottom_left, glm::vec2 top_right);

    /**
     * @pre left <= right
     * @pre bottom <= top
     */
    box(float left, float right, float bottom, float top);

    ~box();

    float left() const {return _bottom_left.x;}
    float right() const {return _top_right.x;}
    float bottom() const {return _bottom_left.y;}
    float top() const {return _top_right.y;}

    glm::vec2 bottom_left() const {return _bottom_left;}
    glm::vec2 top_right() const {return _top_right;}

    float width() const {return right() - left();}
    float height() const {return top() - bottom();}
    glm::vec2 get_dimensions() const {return _top_right - _bottom_left;}

    /**
     * @return true if the given point is in the box.
     */
    bool contains(glm::vec2 p) const;

    /**
     * @return true if the given box intersects this.
     */
    bool intersects(const box& other) const;
};

}}

#endif

