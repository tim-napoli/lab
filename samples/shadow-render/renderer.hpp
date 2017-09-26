#ifndef _shadow_render_renderer_hpp_
#define _shadow_render_renderer_hpp_

#include "lab/lab.hpp"

#define BLUR_NUMBER_OF_PASSES   3

class renderer : public lab::gfx::pipeline {
  private:
    lab::data::manifest   _manifest;

    lab::gfx::program     _canvas_prg;
    lab::gfx::program     _shadows_prg;
    lab::gfx::program     _blur_intermediate_prg;
    lab::gfx::program     _blur_mix_prg;
    lab::gfx::program     _screen_prg;

    lab::gfx::image       _canvas_image;
    lab::gfx::image       _shadows_image;
    lab::gfx::animation   _mime_walk;
    lab::gfx::light       _projection_light;
    glm::vec3             _light_position;

    lab::gfx::image_drawable _canvas_image_drawable;
    lab::gfx::image_drawable _shadows_image_drawable;

    void build_pipeline();

  public:
    renderer(int virtual_width, int virtual_height);

    ~renderer();

    void start(lab::engine::engine_interface* intf)
        throw(lab::util::exception);

    void render() throw(lab::util::exception);

    void stop(lab::engine::engine_interface* intf)
        throw(lab::util::exception);

};

#endif

