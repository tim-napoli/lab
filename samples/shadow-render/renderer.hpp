#ifndef _shadow_render_renderer_hpp_
#define _shadow_render_renderer_hpp_

#include "lab/lab.hpp"

#define BLUR_NUMBER_OF_PASSES   3

class renderer : public lab::gfx::renderer {
  private:
    lab::gfx::light       _projection_light;

    lab::gfx::framebuffer _canvas_fb;
    lab::gfx::image       _canvas_image;
    lab::gfx::program     _canvas_prg;

    lab::gfx::framebuffer _shadows_fb;
    lab::gfx::image       _shadows_image;
    lab::gfx::program     _shadows_prg;

    lab::gfx::framebuffer _blur_intermediate_fbs[BLUR_NUMBER_OF_PASSES];
    lab::gfx::program     _blur_intermediate_prg;
    lab::gfx::framebuffer _blur_fb;
    lab::gfx::program     _blur_mix_prg;

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

