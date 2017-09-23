#include "lab/gfx/draw-pass.hpp"

namespace lab { namespace gfx {

draw_pass::draw_pass(program& prg, framebuffer* output)
        : pass(prg, output)
        , _waiting_queue()
{

}

draw_pass::~draw_pass() {

}

void draw_pass::add_drawable(drawable* d) {
    _waiting_queue.push_back(d);
}

void draw_pass::render() throw(util::exception) {
    for (auto it = _waiting_queue.begin(); it != _waiting_queue.end(); it++) {
        (*it)->bind(get_program());
        (*it)->draw();
    }
    _waiting_queue.clear();
}

}}

