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
    _light_position = glm::vec3(VWIDTH / 2, VHEIGHT / 2, 200);
    _projection_light = gfx::light(
        _light_position,
        100.0f,
        glm::vec3(0.8, 0.5, 0.3)
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

}

void renderer::shadow_render_pass_blur() {

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

    use_screen();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    _screen_prg.use();
    _screen_prg.set_uniform_sampler2d("canvas_tex", 0);
    _canvas_fb.use_texture(GL_TEXTURE0);
    _screen_points.draw();
}

void renderer::stop(engine::engine_interface* intf)
    throw(util::exception)
{
    _canvas_prg.destroy();
    _canvas_image.destroy();
    _canvas_fb.destroy();

    _screen_prg.destroy();
    _screen_points.destroy();
}

