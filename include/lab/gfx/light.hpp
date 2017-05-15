/**
 * @module      lab::gfx::light
 * @brief       The engine gfx light class.
 * @author      Timothée Napoli
 * @license     WTFPL
 * @copyright   Timothée Napoli (c) 2016
 */
#ifndef _lab_gfx_light_hpp_
#define _lab_gfx_light_hpp_

#include <glm/glm.hpp>

namespace lab { namespace gfx {

/**
 * A light is a simple object having a position and a color.
 * For now, the light is only a poncutal one, and it it not attached to
 * any specific features.
 * This will surely change in a near future.
 */
class light {
  private:
    glm::vec3 _position;
    glm::vec3 _color;

  public:
    light();

    light(glm::vec3 position, glm::vec3 color);

    ~light();

    glm::vec3 get_position() const {return _position;}

    glm::vec3 get_color() const {return _color;}

    void set_position(glm::vec3 position) {_position = position;}
};

}}

#endif

