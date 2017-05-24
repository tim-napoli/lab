#include <algorithm>
#include "lab/lab.hpp"

#define DATA_PATH   "data/shadow-render/"
#define VWIDTH      1600
#define VHEIGHT     900

using namespace lab;

// point ----------------------------------------------------------------------
class point {
  private:
    glm::vec2 _position;
    glm::vec2 _speed;

  public:
    point(glm::vec2 pos, glm::vec2 speed)
            : _position(pos)
            , _speed(speed)
    {

    }

    point() : _position(0), _speed(0) {}

    ~point() {

    }

    void move() {
        glm::vec2 dest = _position + _speed;
        if (dest.x < 0) {
            dest.x = 0;
            _speed.x = -_speed.x;
        } else
        if (dest.x > VWIDTH) {
            dest.x = VWIDTH;
            _speed.x = -_speed.x;
        }

        if (dest.y < 0) {
            dest.y = 0;
            _speed.y = -_speed.y;
        } else
        if (dest.y > VHEIGHT) {
            dest.y = VHEIGHT;
            _speed.y = -_speed.y;
        }
        _position = dest;
    }

    glm::vec2 get_position() const {return _position;}

    static point build_random() {
        return point(glm::vec2(rand() % VWIDTH, rand() % VHEIGHT),
                     glm::vec2(-10.0 + rand() % 20, -10.0 + rand() % 20));
    }
};

// hello screen ---------------------------------------------------------------
class hello_screen : public engine::screen {
  private:
    gfx::renderer      _renderer;
    gfx::program       _program;
    gfx::program       _shadow_program;
    gfx::program       _blur_program;
    gfx::program       _mix_program;
    gfx::program       _screen_program;
    gfx::program       _filter_program;
    gfx::program       _light_program;
    gfx::framebuffer   _shadow_fb;
    gfx::image         _canvas;
    gfx::light         _light;
    point              _light_point;
    gfx::image         _joe_image;
    gfx::image         _whole_scene;
    gfx::framebuffer   _canvas_fb;
    gfx::vertex_buffer _post_points;
    gfx::vertex_buffer _bloom_points;

  public:
    hello_screen() : _renderer(VWIDTH, VHEIGHT) { }

    ~hello_screen() { }

// hello-screen::start --------------------------------------------------------
    void start(engine::engine_interface* intf) throw(util::exception) {
        _renderer.set_windows_dimensions(
            intf->get_window_width(),
            intf->get_window_height()
        );

        _program = gfx::program::load(
            DATA_PATH"shader.vert",
            DATA_PATH"shader.frag"
        );
        _shadow_program = gfx::program::load(
            DATA_PATH"shadow.vert",
            DATA_PATH"shadow.frag"
        );
        _blur_program = gfx::program::load(
            DATA_PATH"blur.vert",
            DATA_PATH"blur.frag"
        );
        _mix_program = gfx::program::load(
            DATA_PATH"mix.vert",
            DATA_PATH"mix.frag"
        );
        _screen_program = gfx::program::load(
            DATA_PATH"screen.vert",
            DATA_PATH"screen.frag"
        );
        _filter_program = gfx::program::load(
            DATA_PATH"filter.vert",
            DATA_PATH"filter.frag"
        );
        _light_program = gfx::program::load(
            DATA_PATH"light.vert",
            DATA_PATH"light.frag"
        );

        _canvas = gfx::image(
            (std::vector<gfx::texture>) {
                gfx::texture::load(DATA_PATH"canvas.png"),
                gfx::texture::load(DATA_PATH"canvas-normal.png")
            },
            glm::vec2(0),
            glm::vec2(VWIDTH, VHEIGHT)
        );
        _canvas_fb = gfx::framebuffer(VWIDTH, VHEIGHT);
        _shadow_fb = gfx::framebuffer(VWIDTH, VHEIGHT);

        _joe_image = gfx::image(
            (std::vector<gfx::texture>) {
                gfx::texture::load(DATA_PATH"joe.png")
            },
            glm::vec2(167, 436),
            glm::vec2(316, 436)
        );

        _whole_scene = gfx::image(
            (std::vector<gfx::texture>) {
                gfx::texture::load(DATA_PATH"whole-scene.png")
            },
            glm::vec2(0, 0),
            glm::vec2(1600, 900)
        );

        _light = gfx::light(
            glm::vec3(800, 400, 1),
            glm::vec3(0.8, 0.5, 0.3)
        );
        _light_point = point::build_random();

        _post_points = gfx::vertex_buffer(
            gfx::vertex_buffer::static_draw,
            (std::vector<glm::vec2>) {
                glm::vec2(0.0, 0.0),        glm::vec2(0.0, 0.0),
                glm::vec2(VWIDTH, 0.0),     glm::vec2(1.0, 0.0),
                glm::vec2(VWIDTH, VHEIGHT), glm::vec2(1.0, 1.0),

                glm::vec2(0.0, 0.0),        glm::vec2(0.0, 0.0),
                glm::vec2(0.0, VHEIGHT),    glm::vec2(0.0, 1.0),
                glm::vec2(VWIDTH, VHEIGHT), glm::vec2(1.0, 1.0),
        }, 2);

        _bloom_points = gfx::vertex_buffer(
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

// hello-screen::render -------------------------------------------------------

    void render_canvas() {
        _renderer.use_framebuffer(_canvas_fb);

        _program.set_uniform_mat4(
            "projection_matrix", _renderer.get_projection_matrix_ptr()
        );
        _program.set_uniform_vec3(
            "light_position", _light.get_position()
        );
        _program.set_uniform_vec3(
            "light_color", _light.get_color()
        );
        _program.set_uniform_sampler2d(
            "diffuse_map", 0
        );
        _program.set_uniform_sampler2d(
            "normal_map", 1
        );

        _program.use();
        _canvas.draw();
    }

    void render_shadows() {
        _renderer.use_framebuffer(_shadow_fb);
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        // First, we do light on the framebuffer.
        _light_program.use();
        _light_program.set_uniform_mat4(
            "projection_matrix", _renderer.get_projection_matrix_ptr()
        );
        _light_program.set_uniform_vec3(
            "light_position", _light.get_position()
        );
        _light_program.set_uniform_vec3(
            "light_color", _light.get_color()
        );
        _post_points.draw();

        _shadow_program.use();
        _shadow_program.set_uniform_mat4(
            "projection_matrix", _renderer.get_projection_matrix_ptr()
        );
        _shadow_program.set_uniform_sampler2d(
            "tex", 0
        );
        //_joe_image.draw();
        glEnable(GL_BLEND);
        glBlendFunc(GL_DST_COLOR, GL_ZERO);
        _whole_scene.draw();
        glDisable(GL_BLEND);
    }

    void bloom_effect() {
        /*
        // First apply a brightpass on the image. We only conserve values
        // that are above a given luminosity thresold.
        gfx::framebuffer filtered_fb(VWIDTH, VHEIGHT);
        _renderer.use_framebuffer(filtered_fb);
        _filter_program.use();
        _canvas_fb.use_texture(GL_TEXTURE0);
        _bloom_points.draw();
        */

        // We get 2-divisions framebuffers.
        std::vector<gfx::framebuffer> layers = {
            gfx::framebuffer(VWIDTH, VHEIGHT),
            gfx::framebuffer(VWIDTH / 2, VHEIGHT / 2),
            gfx::framebuffer(VWIDTH / 4, VHEIGHT / 4),
            gfx::framebuffer(VWIDTH / 8, VHEIGHT / 8),
        };

        // Apply blur on every layers.
        _shadow_fb.use_texture(GL_TEXTURE0);
        std::for_each(layers.begin(), layers.end(),
            [this](gfx::framebuffer& fb) {
                _renderer.use_framebuffer(fb);
                _blur_program.use();
                _bloom_points.draw();
            }
        );

        gfx::framebuffer bloom_fb(VWIDTH, VHEIGHT);

        // Combining layers into one.
        _renderer.use_framebuffer(bloom_fb);
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        _mix_program.use();
        glEnable(GL_BLEND);
        glBlendFunc(GL_ZERO, GL_SRC_COLOR);
        std::for_each(layers.begin(), layers.end(),
            [this](gfx::framebuffer& fb) {
                fb.use_texture(GL_TEXTURE0);
                _bloom_points.draw();
            }
        );
        glDisable(GL_BLEND);

        /*
        _renderer.use_framebuffer(_canvas_fb);
        _shadow_program.use();
        glEnable(GL_BLEND);
        glBlendFunc(GL_DST_COLOR, GL_ZERO);
        _whole_scene.draw();
        glDisable(GL_BLEND);
        */

        // Combining the bloom framebuffer with the canvas framebuffer.
        _renderer.use_screen();
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        #if 1
        _screen_program.use();
        _screen_program.set_uniform_sampler2d("canvas_tex", 0);
        _screen_program.set_uniform_sampler2d("bloom_tex", 1);
        _canvas_fb.use_texture(GL_TEXTURE0);
        bloom_fb.use_texture(GL_TEXTURE1);
        #else
        _mix_program.use();
        _canvas_fb.use_texture(GL_TEXTURE0);
        #endif
        _bloom_points.draw();

        bloom_fb.destroy();
        std::for_each(layers.begin(), layers.end(), [](gfx::framebuffer& fb) {
            fb.destroy();
        });
    }

    void render_posteffects() {
        bloom_effect();
    }

    void render() {
        render_canvas();
        render_shadows();
        render_posteffects();
    }

// render-screen::other -------------------------------------------------------
    void update(engine::engine_interface* intf) throw(util::exception) {
        _light_point.move();
        _light.set_position(glm::vec3(_light_point.get_position(), 300.0));

        render();
    }

    void stop(engine::engine_interface* intf) throw(util::exception) {
        _program.destroy();
        _shadow_program.destroy();
        _blur_program.destroy();
        _mix_program.destroy();
        _screen_program.destroy();
        _filter_program.destroy();
        _light_program.destroy();

        _canvas.destroy();
        _joe_image.destroy();
        _whole_scene.destroy();

        _canvas_fb.destroy();
        _shadow_fb.destroy();

        _post_points.destroy();
        _bloom_points.destroy();
    }

    void notify(const event::event& evt) throw(util::exception) {
        if (evt.get_source_id() == engine::window::get_id()) {
            if (evt.get_type() == engine::window::events::resized) {
                _renderer.set_windows_dimensions(
                    evt.get_value("width").get<int>(),
                    evt.get_value("height").get<int>()
                );
            }
        }
    }
};

// ----------------------------------------------------------------------------

int main(void) {
    engine::engine engine("shadow render", 1024, 768, false, 60);

    try {
        engine.start();
        engine.start_screen(std::unique_ptr<engine::screen>(new hello_screen()));
        engine.run();
        engine.stop();
    } catch (util::exception ex) {
        std::cout << ex.get_message() << std::endl;
    }

    return 0;
}

