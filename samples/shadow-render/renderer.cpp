#include "constants.hpp"
#include "renderer.hpp"

using namespace lab;

renderer::renderer(int virtual_width, int virtual_height)
    : gfx::renderer (virtual_width, virtual_height)
{

}
renderer::~renderer() {

}

void renderer::start(engine::engine_interface* intf)
    throw(util::exception)
{
    set_windows_dimensions(
        intf->get_window_width(),
        intf->get_window_height()
    );

    _light_position = glm::vec3(VWIDTH / 2, VHEIGHT / 2, 200);
    _projection_light = gfx::light(
        _light_position,
        100.0f,
        glm::vec3(0.8, 0.5, 0.1)
    );

    _canvas_fb = gfx::framebuffer(VWIDTH, VHEIGHT);
    _canvas_image = gfx::image(
        (std::vector<gfx::texture>) {
            gfx::texture::load(DATA_PATH"canvas.png"),
            gfx::texture::load(DATA_PATH"canvas-normal.png")
        },
        glm::vec2(0),
        glm::vec2(VWIDTH, VHEIGHT)
    );
    _canvas_prg = gfx::program::load(
        DATA_PATH"canvas.vert",
        DATA_PATH"canvas.frag"
    );

    _shadows_fb = gfx::framebuffer(VWIDTH, VHEIGHT);
    _shadows_image = gfx::image(
        (std::vector<gfx::texture>) {
            gfx::texture::load(DATA_PATH"whole-scene.png")
        },
        glm::vec2(0),
        glm::vec2(VWIDTH, VHEIGHT)
    );
    _shadows_prg = gfx::program::load(
        DATA_PATH"shadows.vert",
        DATA_PATH"shadows.frag"
    );

    for (int i = 0; i < BLUR_NUMBER_OF_PASSES; i++) {
        _blur_intermediate_fbs[i] = gfx::framebuffer(
            VWIDTH >> i, VHEIGHT >> i
        );
    }
    _blur_intermediate_prg = gfx::program::load(
        DATA_PATH"blur.vert",
        DATA_PATH"blur.frag"
    );
    _blur_fb = gfx::framebuffer(VWIDTH, VHEIGHT);
    _blur_mix_prg = gfx::program::load(
        DATA_PATH"mix.vert",
        DATA_PATH"mix.frag"
    );

    _screen_prg = gfx::program::load(
        DATA_PATH"screen.vert",
        DATA_PATH"screen.frag"
    );
    _screen_points = gfx::vertex_buffer(
        gfx::vertex_buffer::static_draw,
        (std::vector<glm::vec2>) {
            glm::vec2(-1.0, -1.0), glm::vec2(0.0, 0.0),
            glm::vec2(+1.0, -1.0), glm::vec2(1.0, 0.0),
            glm::vec2(+1.0, +1.0), glm::vec2(1.0, 1.0),

            glm::vec2(-1.0, -1.0), glm::vec2(0.0, 0.0),
            glm::vec2(-1.0, +1.0), glm::vec2(0.0, 1.0),
            glm::vec2(+1.0, +1.0), glm::vec2(1.0, 1.0),
        }, 2
    );
}

void renderer::canvas_render_pass() {
        use_framebuffer(_canvas_fb);

        _canvas_prg.set_uniform_mat4(
            "projection_matrix", get_projection_matrix_ptr()
        );
        _canvas_prg.set_uniform_vec3(
            "light_position", _projection_light.get_position()
        );
        _canvas_prg.set_uniform_vec3(
            "light_color", _projection_light.get_color()
        );
        _canvas_prg.set_uniform_sampler2d(
            "diffuse_map", 0
        );
        _canvas_prg.set_uniform_sampler2d(
            "normal_map", 1
        );

        _canvas_prg.use();
        _canvas_image.draw();
}

void renderer::shadow_render_pass_shadows() {
    use_framebuffer(_shadows_fb);
    _shadows_prg.set_uniform_mat4(
        "projection_matrix", get_projection_matrix_ptr()
    );
    _shadows_prg.set_uniform_sampler2d(
        "tex", 0
    );
    _shadows_prg.use();

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    _shadows_image.draw();
}

void renderer::shadow_render_pass_blur() {
    _shadows_fb.use_texture(GL_TEXTURE0);
    _blur_intermediate_prg.set_uniform_sampler2d(
        "tex", 0
    );
    _blur_intermediate_prg.use();
    for (int i = 0; i < BLUR_NUMBER_OF_PASSES; i++) {
        use_framebuffer(_blur_intermediate_fbs[i]);
        _screen_points.draw();
    }

    use_framebuffer(_blur_fb);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    _blur_mix_prg.set_uniform_sampler2d(
        "tex", 0
    );
    _blur_mix_prg.use();
    glEnable(GL_BLEND);
    glBlendFunc(GL_ZERO, GL_SRC_COLOR);
    for (int i = 0; i < BLUR_NUMBER_OF_PASSES; i++) {
        _blur_intermediate_fbs[i].use_texture(GL_TEXTURE0);
        _screen_points.draw();
    }
    glDisable(GL_BLEND);
}

void renderer::shadow_render_pass() {
    shadow_render_pass_shadows();
    shadow_render_pass_blur();
}

void renderer::render(glm::vec2 light_pos) {
    _projection_light.set_position(glm::vec3(light_pos, 200));

    // Rendering is done in multiple passes :
    // First, we need to render the canvas with the illumination (projection
    // light). We store that rendering in a framebuffer.
    // Next, we need to render the shadows in an other framebuffer, and to
    // apply the blur effect on it.
    // Finally, we will draw to the screen the mix of the canvas and the
    // shadows frambeuffers on the screen.
    canvas_render_pass();
    shadow_render_pass();

    use_screen();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    _screen_prg.use();
    _screen_prg.set_uniform_sampler2d("canvas_tex", 0);
    _screen_prg.set_uniform_sampler2d("shadows_tex", 1);
    _canvas_fb.use_texture(GL_TEXTURE0);
    _blur_fb.use_texture(GL_TEXTURE1);
    _screen_points.draw();
}

void renderer::stop(engine::engine_interface* intf)
    throw(util::exception)
{
    _canvas_prg.destroy();
    _canvas_image.destroy();
    _canvas_fb.destroy();

    _shadows_fb.destroy();
    _shadows_image.destroy();
    _shadows_fb.destroy();

    for (int i = 0; i < BLUR_NUMBER_OF_PASSES; i++) {
        _blur_intermediate_fbs[i].destroy();
    }
    _blur_intermediate_prg.destroy();
    _blur_fb.destroy();
    _blur_mix_prg.destroy();

    _screen_prg.destroy();
    _screen_points.destroy();
}

