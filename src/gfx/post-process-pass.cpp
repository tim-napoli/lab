#include "lab/gfx/post-process-pass.hpp"

namespace lab { namespace gfx {

post_process_pass::post_process_pass(program& prg, framebuffer* output,
                                     math::box box)
        : pass(prg, output)
{
    std::vector<glm::vec2> points = {
        glm::vec2(box.left(),  box.bottom()), glm::vec2(0, 0),
        glm::vec2(box.right(), box.bottom()), glm::vec2(1.0, 0),
        glm::vec2(box.right(), box.top()),    glm::vec2(1.0, 1.0),

        glm::vec2(box.left(),  box.bottom()), glm::vec2(0, 0),
        glm::vec2(box.left(),  box.top()),    glm::vec2(0.0, 1.0),
        glm::vec2(box.right(), box.top()),    glm::vec2(1.0, 1.0)
    };
    _vertices = vertex_buffer(
        vertex_buffer::type::static_draw,
        points,
        2
    );
}

post_process_pass::~post_process_pass() {
    _vertices.destroy();
}

void post_process_pass::add_input(const std::string& name,
                                  const framebuffer* fb)
{
    _input_fbs.emplace(name, fb);
}

void post_process_pass::render() throw(util::exception) {
    GLuint id = 0;
    for (auto it = _input_fbs.begin(); it != _input_fbs.end(); it++) {
        get_program().set_uniform_sampler2d(it->first, id);
        it->second->bind_texture(GL_TEXTURE0 + id);
        id++;
    }
    _vertices.draw();
}

}}

