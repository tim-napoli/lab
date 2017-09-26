#include "constants.hpp"
#include "renderer.hpp"

#include <glm/gtc/matrix_transform.hpp>

using namespace lab;

renderer::renderer(int virtual_width, int virtual_height)
    : gfx::pipeline(virtual_width, virtual_height)
{

}
renderer::~renderer() {

}

void renderer::build_pipeline() {
    gfx::draw_pass* canvas_pass = new gfx::draw_pass(
        _canvas_prg, new gfx::framebuffer(VWIDTH, VHEIGHT)
    );
    gfx::draw_pass* shadows_pass = new gfx::draw_pass(
        _shadows_prg, new gfx::framebuffer(VWIDTH, VHEIGHT)
    );
    // In these passes, input vertices must be in [-1.0, 1.0]
    gfx::post_process_pass* blur_1_pass = new gfx::post_process_pass(
        _blur_intermediate_prg, new gfx::framebuffer(VWIDTH, VHEIGHT),
        math::box(-1.0, 1.0, -1.0, 1.0)
    );
    blur_1_pass->add_input("shadows", shadows_pass->get_output());

    gfx::post_process_pass* blur_2_pass = new gfx::post_process_pass(
        _blur_intermediate_prg, new gfx::framebuffer(VWIDTH / 2, VHEIGHT / 2),
        math::box(-1.0, 1.0, -1.0, 1.0)
    );
    blur_2_pass->add_input("shadows", shadows_pass->get_output());

    gfx::post_process_pass* blur_3_pass = new gfx::post_process_pass(
        _blur_intermediate_prg, new gfx::framebuffer(VWIDTH / 4, VHEIGHT / 4),
        math::box(-1.0, 1.0, -1.0, 1.0)
    );
    blur_3_pass->add_input("shadows", shadows_pass->get_output());

    gfx::post_process_pass* blur_mix_pass = new gfx::post_process_pass(
        _blur_mix_prg, new gfx::framebuffer(VWIDTH, VHEIGHT),
        math::box(-1.0, 1.0, -1.0, 1.0)
    );
    blur_mix_pass->add_input("blur_1", blur_1_pass->get_output());
    blur_mix_pass->add_input("blur_2", blur_2_pass->get_output());
    blur_mix_pass->add_input("blur_3", blur_3_pass->get_output());

    gfx::post_process_pass* screen_pass = new gfx::post_process_pass(
        _screen_prg, NULL, math::box(-1.0, 1.0, -1.0, 1.0)
    );
    screen_pass->add_input("canvas_texture", canvas_pass->get_output());
    screen_pass->add_input("shadows_texture", blur_mix_pass->get_output());

    add_draw_pass("canvas_pass", canvas_pass);
    add_draw_pass("shadows_pass", shadows_pass);
    add_post_process_pass("blur_1_pass", blur_1_pass);
    add_post_process_pass("blur_2_pass", blur_2_pass);
    add_post_process_pass("blur_3_pass", blur_3_pass);
    add_post_process_pass("blur_mix_pass", blur_mix_pass);
    add_post_process_pass("screen_pass", screen_pass);
}

void renderer::start(engine::engine_interface* intf)
    throw(util::exception)
{
    set_windows_dimensions(
        intf->get_window_width(),
        intf->get_window_height()
    );

    _manifest = data::manifest::load(DATA_PATH"manifest.json");

    _light_position = glm::vec3(VWIDTH / 2, VHEIGHT / 2, 200);
    _projection_light = gfx::light(
        _light_position,
        100.0f,
        glm::vec3(0.8, 0.5, 0.1)
    );

    _canvas_prg = gfx::program::load(
        DATA_PATH"shaders/canvas.vert",
        DATA_PATH"shaders/canvas.frag"
    );
    _shadows_prg = gfx::program::load(
        DATA_PATH"shaders/shadows.vert",
        DATA_PATH"shaders/shadows.frag"
    );
    _blur_intermediate_prg = gfx::program::load(
        DATA_PATH"shaders/blur.vert",
        DATA_PATH"shaders/blur.frag"
    );
    _blur_mix_prg = gfx::program::load(
        DATA_PATH"shaders/mix.vert",
        DATA_PATH"shaders/mix.frag"
    );
    _screen_prg = gfx::program::load(
        DATA_PATH"shaders/screen.vert",
        DATA_PATH"shaders/screen.frag"
    );

    _mime_walk = _manifest.get_animation("mime-walk");

    _canvas_image_drawable = gfx::image_drawable(
        glm::mat3(), _manifest.get_image_ptr("canvas"),
        {"diffuse_map", "normal_map"}
    );
    _shadows_image_drawable = gfx::image_drawable(
        glm::mat3(), _manifest.get_image_ptr("whole-scene"),
        {"shadow_texture"}
    );

    build_pipeline();
}

void renderer::render() throw(util::exception) {
    _projection_light.set_position(glm::vec3(VWIDTH / 2, VHEIGHT / 2, 200));
    _mime_walk.update();

    get_draw_pass("shadows_pass")->add_drawable(&_shadows_image_drawable);
    get_draw_pass("canvas_pass")->add_drawable(&_canvas_image_drawable);
    _canvas_prg.set_uniform_vec3("light_position", _light_position);
    _canvas_prg.set_uniform_vec3("light_color", _projection_light.get_color());

    try {
        gfx::pipeline::render();
    } catch (util::exception ex) {
        std::cerr << ex.get_message() << std::endl;
    }
}

void renderer::stop(engine::engine_interface* intf)
    throw(util::exception)
{
    _shadows_prg.destroy();
    _canvas_prg.destroy();
    _blur_intermediate_prg.destroy();
    _blur_mix_prg.destroy();
    _screen_prg.destroy();

    _manifest.destroy();
}

