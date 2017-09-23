#include "lab/gfx/pass.hpp"

namespace lab { namespace gfx {

pass::pass(program& prg, framebuffer* output)
        : _program(prg)
        , _output_fb(output)
{

}

pass::~pass() {
    if (_output_fb) {
        _output_fb->destroy();
        delete _output_fb;
    }
}

}}

