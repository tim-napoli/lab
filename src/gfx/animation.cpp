#include "lab/gfx/animation.hpp"

namespace lab { namespace gfx {

animation_frame::animation_frame(image img, unsigned int tick_duration)
        : _image(img)
        , _tick_duration(tick_duration)
{

}

animation_frame::~animation_frame() {

}

void animation_frame::draw() const {
    _image.draw();
}

void animation_frame::destroy() {
    _image.destroy();
}

animation::animation()
        : _frames()
        , _current_frame(0)
        , _counter(0)
{

}

animation::animation(const std::vector<animation_frame>& frames)
        : _frames(frames)
        , _current_frame(0)
        , _counter(0)
{

}

animation::~animation() {

}

void animation::update() {
    _counter++;
    if (_counter >= _frames[_current_frame].get_tick_duration()) {
        _counter = 0;
        _current_frame = (_current_frame + 1) % _frames.size();
    }
}

void animation::draw() const {
    _frames[_current_frame].draw();
}

void animation::destroy() {
    for (auto it = _frames.begin(); it != _frames.end(); it++) {
        it->destroy();
    }
}

}}

