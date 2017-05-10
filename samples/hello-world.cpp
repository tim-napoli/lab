#include "lab/lab.hpp"

using namespace lab;

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

    ~point() {

    }

    void move() {
        glm::vec2 dest = _position + _speed;
        if (dest.x < 0) {
            dest.x = 0;
            _speed.x = -_speed.x;
        } else
        if (dest.x > 16000) {
            dest.x = 16000;
            _speed.x = -_speed.x;
        }

        if (dest.y < 0) {
            dest.y = 0;
            _speed.y = -_speed.y;
        } else
        if (dest.y > 8000) {
            dest.y = 8000;
            _speed.y = -_speed.y;
        }
        _position = dest;
    }

    glm::vec2 get_position() const {return _position;}

    static point build_random() {
        return point(glm::vec2(rand() % 16000, rand() % 8000),
                     glm::vec2(-100.0 + rand() % 200, -100.0 + rand() % 200));
    }
};

class hello_screen : public engine::screen {
  private:
    gfx::renderer      _renderer;
    gfx::program       _program_first;
    gfx::program       _program_second;
    gfx::vertex_buffer _vertexes;
    gfx::vertex_buffer _quad_vertexes;
    gfx::framebuffer   _framebuf;
    gfx::texture       _canvas;
    gfx::texture       _canvas_normal;

    std::vector<point> _points;

  public:
    hello_screen() : _renderer(1600, 800) { }

    ~hello_screen() { }

    void start(engine::engine_interface* intf) throw(util::exception) {
        _renderer.set_windows_dimensions( intf->get_window_width()
                                        , intf->get_window_height()
                                        );
        _program_first = gfx::program::load(
            "data/hello-first.vert",
            "data/hello-first.frag"
        );
        _program_second = gfx::program::load(
            "data/hello-second.vert",
            "data/hello-second.frag"
        );

        _points.push_back(point::build_random());
        _points.push_back(point::build_random());
        _points.push_back(point::build_random());

        _vertexes = gfx::vertex_buffer(
            gfx::vertex_buffer::dynamic_draw,
            (std::vector<glm::vec2>){
                _points[0].get_position(),
                _points[1].get_position(),
                _points[2].get_position()
            }, 1
        );
        _quad_vertexes = gfx::vertex_buffer(
            gfx::vertex_buffer::static_draw,
            (std::vector<glm::vec2>){
                glm::vec2(0,   0),
                glm::vec2(1600, 0),
                glm::vec2(1600, 800),

                glm::vec2(0, 0),
                glm::vec2(0, 800),
                glm::vec2(1600, 800)
            }, 1
        );

        _framebuf = gfx::framebuffer(1600, 800);

        _canvas = gfx::texture::load("data/screen-1.png");
        _canvas_normal = gfx::texture::load("data/screen-1-normal.png");
    }

    void render() {
        // First pass
        _renderer.use_framebuffer(_framebuf);
        glClearColor(1.0,  1.0,  1.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _framebuf.get_texture_id());
        _program_first.set_uniform_mat4(
            "projection_matrix", _renderer.get_projection_matrix_ptr()
        );
        _program_first.use();
        _vertexes.draw();

        // Second pass
        _renderer.use_screen();
        glClearColor(0.0,  0.0,  0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        _program_second.set_uniform_mat4(
            "projection_matrix", _renderer.get_projection_matrix_ptr()
        );
        _program_second.set_uniform_sampler2d(
            "first_texture", 0
        );
        _program_second.set_uniform_sampler2d(
            "canvas_texture", 1
        );
        _program_second.use();
        _framebuf.use_texture(GL_TEXTURE0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, _canvas.get_texture_id());
        glEnable(GL_BLEND);
        _quad_vertexes.draw();
        glDisable(GL_BLEND);
    }

    void update(engine::engine_interface* intf) throw(util::exception) {
        for (auto it = _points.begin(); it != _points.end(); it++) {
            it->move();
        }
        _vertexes.update_vertexes(
            (std::vector<glm::vec2>){
                _points[0].get_position(),
                _points[1].get_position(),
                _points[2].get_position()
            }
        );

        render();
    }

    void stop(engine::engine_interface* intf) throw(util::exception) {
        _vertexes.destroy();
        _quad_vertexes.destroy();
        _program_first.destroy();
        _program_second.destroy();
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

int main(void) {
    engine::engine engine("hallo world", 1024, 768, false, 60);

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

