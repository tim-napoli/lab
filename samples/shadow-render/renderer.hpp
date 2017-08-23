#ifndef _shadow_render_renderer_hpp_
#define _shadow_render_renderer_hpp_

#include "lab/lab.hpp"

class renderer : public lab::gfx::renderer {
  private:
    lab::gfx::framebuffer _canvas_fb;
    lab::gfx::image       _canvas_image;
    lab::gfx::light       _projection_light;
    lab::gfx::program     _canvas_prg;

    lab::gfx::program       _screen_prg;
    lab::gfx::vertex_buffer _screen_points;

    glm::vec3 _light_position;

    void canvas_render_pass();
    void shadow_render_pass_shadows();
    void shadow_render_pass_blur();
    void shadow_render_pass();

  public:
    renderer(int virtual_width, int virtual_height);

    ~renderer();

    void start(lab::engine::engine_interface* intf)
        throw(lab::util::exception);

    void render(glm::vec2 light_pos);

    void stop(lab::engine::engine_interface* intf)
        throw(lab::util::exception);

};

#endif

