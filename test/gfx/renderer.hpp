#include <unistd.h>
#include <thread>
#include <cxxtest/TestSuite.h>
#include "mocks/gl.hpp"
#include "lab/engine/engine.hpp"
#include "lab/engine/window.hpp"
#include "lab/gfx/renderer.hpp"

using namespace lab;

class renderer_monitor : public engine::module {
  private:
    size_t _resize_count;

  public:
    renderer_monitor() : engine::module("renderer_monitor", json::json())
                       , _resize_count(0)
    {

    }

    ~renderer_monitor() {

    }

    void start() throw(util::exception) {

    }

    void stop() throw(util::exception) {

    }

    void update() throw(util::exception) {

    }

    size_t get_resize_count() const {return _resize_count;}

    void notify(const event::event& evt) throw(util::exception) {
        if (evt.get_source_id() != gfx::renderer::get_id()) {
            return;
        }

        if (evt.get_type() == gfx::renderer::events::resized) {
            _resize_count++;
            glm::mat4 mat = evt.get_value("real_to_virt").get_raw<glm::mat4>();

            if (_resize_count == 1) {
                glm::vec4 click((float)(rand() % 400), (float)(rand() % 300), 0.0f, 1.0f);
                glm::vec4 transposed = mat * click;

                TS_ASSERT(transposed.x == click.x * 0.5);
                TS_ASSERT(transposed.y == (click.y - 50) * 0.5);
            }
        }
    }
};

class RendererTestSuite : public CxxTest::TestSuite {
  public:
    void setUp() {
    }

    void tearDown() {
    }

    void testRenderer() {
        engine::engine engine(60);
        std::unique_ptr<engine::window> window_ptr(
            new engine::window("test", json::json({
                {"width", util::value::build<int>(400)},
                {"height", util::value::build<int>(300)},
                {"fullscreen", util::value::build<bool>(false)}
            }))
        );
        std::unique_ptr<gfx::renderer> renderer_ptr(
            new gfx::renderer(json::json({
                {"virtual_width", util::value::build<int>(800)},
                {"virtual_height", util::value::build<int>(400)}
            }))
        );
        renderer_monitor* monitor = new renderer_monitor();
        std::unique_ptr<renderer_monitor> monitor_ptr(monitor);

        window_ptr->register_listener(renderer_ptr.get());
        window_ptr->register_listener(&engine);
        renderer_ptr->register_listener(monitor_ptr.get());

        std::thread thread([](engine::window* win) {
            usleep(5000);
            win->resize(500, 300);
            win->close();
        }, window_ptr.get());

        std::cerr << "window id " << engine::window::get_id() << std::endl;
        std::cerr << "renderer id " << gfx::renderer::get_id() << std::endl;

        engine.plug_module(std::move(window_ptr));
        engine.plug_module(std::move(renderer_ptr));
        engine.plug_module(std::move(monitor_ptr));

        try {
            engine.start();
            engine.run();
            engine.stop();
        } catch (util::exception ex) {
            TS_WARN(ex.get_message());
            TS_ASSERT(false);
        }

        thread.join();
        TS_ASSERT(monitor->get_resize_count() == 2);
        TS_ASSERT(gl_mock_viewport.x == 0);
        TS_ASSERT(gl_mock_viewport.y == 25);
        TS_ASSERT(gl_mock_viewport.width == 500);
        TS_ASSERT(gl_mock_viewport.height == 250);
    }

    void testBadConfiguration() {
        engine::engine engine(60);
        std::unique_ptr<gfx::renderer> renderer_ptr(
            new gfx::renderer(json::json({
                {"virtl_width", util::value::build<int>(800)},
                {"virtal_height", util::value::build<int>(400)}
            }))
        );

        engine.plug_module(std::move(renderer_ptr));
        TS_ASSERT_THROWS(engine.start(), util::exception);
    }
};

