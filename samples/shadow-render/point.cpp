#include "constants.hpp"
#include "point.hpp"

point::point(glm::vec2 pos, glm::vec2 speed)
        : _position(pos)
        , _speed(speed)
{

}

point::point() : _position(0), _speed(0) {}

point::~point() {

}

void point::move() {
    glm::vec2 dest = _position + _speed;
    if (dest.x < 0) {
        dest.x = 0;
        _speed.x = -_speed.x;
    } else
    if (dest.x > VWIDTH) {
        dest.x = VWIDTH;
        _speed.x = -_speed.x;
    }

    if (dest.y < 0) {
        dest.y = 0;
        _speed.y = -_speed.y;
    } else
    if (dest.y > VHEIGHT) {
        dest.y = VHEIGHT;
        _speed.y = -_speed.y;
    }
    _position = dest;
}

point point::build_random() {
    return point(glm::vec2(rand() % VWIDTH, rand() % VHEIGHT),
                 glm::vec2(-10.0 + rand() % 20, -10.0 + rand() % 20));
}

