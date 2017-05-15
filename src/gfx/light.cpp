#include "lab/gfx/light.hpp"

namespace lab { namespace gfx {

light::light()
    : _position(glm::vec3(0))
    , _color(glm::vec3(0))
{

}

light::light(glm::vec3 position, glm::vec3 color)
    : _position(position)
    , _color(color)
{

}

light::~light() {

}

}}

