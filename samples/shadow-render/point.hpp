#ifndef _shadow_render_point_hpp_
#define _shadow_render_point_hpp_

#include <glm/glm.hpp>

class point {
  private:
    glm::vec2 _position;
    glm::vec2 _speed;

  public:
    point();
    point(glm::vec2 pos, glm::vec2 speed);

    ~point();

    void move();

    glm::vec2 get_position() const {return _position;}

    static point build_random();
};


#endif

