#include "lab/gfx/light.hpp"

namespace lab { namespace gfx {

light::light()
    : _position(glm::vec3(0))
    , _radius(1.0f)
    , _color(glm::vec3(0))
{

}

light::light(glm::vec3 position, float radius, glm::vec3 color)
    : _position(position)
    , _radius(radius)
    , _color(color)
{

}

light::~light() {

}

}}

