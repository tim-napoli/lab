#include "lab/gfx/pipeline.hpp"
#include "lab/util/string.hpp"

namespace lab { namespace gfx {

pipeline::pipeline(int virtual_width, int virtual_height)
        : renderer(virtual_width, virtual_height)
        , _draw_passes()
        , _post_process_passes()
        , _draw_passes_order()
        , _post_process_passes_order()
{

}

pipeline::~pipeline() {
    for (auto it = _draw_passes_order.begin();
              it != _draw_passes_order.end();
              it++)
    {
        delete *it;
    }
    for (auto it = _post_process_passes_order.begin();
              it != _post_process_passes_order.end();
              it++)
    {
        delete *it;
    }
}

std::string pipeline::get_name_of_draw_pass(draw_pass* p) const {
    for (auto it = _draw_passes.begin();
              it != _draw_passes.end();
              it++)
    {
        if (it->second == p) {
            return it->first;
        }
    }
    return "unknown draw pass";
}

std::string pipeline::get_name_of_post_process_pass(post_process_pass* p) const
{
    for (auto it = _post_process_passes.begin();
              it != _post_process_passes.end();
              it++)
    {
        if (it->second == p) {
            return it->first;
        }
    }
    return "unknown post-process pass";
}

void pipeline::add_draw_pass(const std::string& name, draw_pass* p)
        throw(util::exception)
{
    auto it = _draw_passes.find(name);
    if (it != _draw_passes.end()) {
        throw util::exception(util::format(
            "The pipeline already has a draw pass '{}'", name
        ));
    }
    _draw_passes.emplace(name, p);
    _draw_passes_order.push_back(p);
}

draw_pass* pipeline::get_draw_pass(const std::string& name)
        throw(util::exception)
{
    auto it = _draw_passes.find(name);
    if (it == _draw_passes.end()) {
        throw util::exception(util::format(
            "The pipeline doesn't have a draw pass '{}'", name
        ));
    }
    return it->second;
}

void pipeline::add_post_process_pass(const std::string& name,
                                     post_process_pass* p)
        throw(util::exception)
{
    auto it = _post_process_passes.find(name);
    if (it != _post_process_passes.end()) {
        throw util::exception(util::format(
            "The pipeline already has a post process pass '{}'", name
        ));
    }
    _post_process_passes.emplace(name, p);
    _post_process_passes_order.push_back(p);
}

void pipeline::set_output_framebuffer(pass* p) {
    const framebuffer* out_fb = p->get_output();
    if (out_fb == NULL) {
        use_screen();
    } else {
        use_framebuffer(out_fb);
    }
}

void pipeline::render() throw(util::exception) {
    for (auto it = _draw_passes_order.begin();
              it != _draw_passes_order.end();
              it++)
    {
        set_output_framebuffer(*it);
        (*it)->get_program().use();
        // XXX Is that normal ? After thinking, I guess the projection should
        //     be done when drawing stuffes on the screen, not on a
        //     framebuffer !
        (*it)->get_program().set_uniform_mat4(
            "projection_matrix",
            get_projection_matrix_ptr()
        );
        try {
            (*it)->render();
        } catch (util::exception ex) {
            throw util::exception(util::format(
                "Pipeline draw pass '{}' rendering error: {}",
                get_name_of_draw_pass(*it), ex.get_message()
            ));
        }
    }

    for (auto it = _post_process_passes_order.begin();
              it != _post_process_passes_order.end();
              it++)
    {
        set_output_framebuffer(*it);
        (*it)->get_program().use();
        try {
            (*it)->render();
        } catch (util::exception ex) {
            throw util::exception(util::format(
                "Pipeline post-process pass '{}' rendering error: {}",
                get_name_of_post_process_pass(*it), ex.get_message()
            ));
        }
    }
}

}}

