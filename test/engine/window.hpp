#include <unistd.h>
#include <thread>
#include <cxxtest/TestSuite.h>
#include "mocks/glfw.hpp"
#include "lab/engine/engine.hpp"
#include "lab/engine/window.hpp"

using namespace lab;

class window_monitor : public engine::module {
  private:
    bool _should_close;
    int  _width;
    int  _height;

  public:
    window_monitor() : engine::module("window_monitor", json::json())
                     , _should_close(false)
                     , _width(0)
                     , _height(0)
    {

    }

    ~window_monitor() {

    }

    bool should_close() const {return _should_close;}
    int get_width() const {return _width;}
    int get_height() const {return _height;}

    void start() throw(util::exception) {

    }

    void stop() throw(util::exception) {

    }

    void update() throw(util::exception) {

    }

    void notify(const event::event& evt) throw(util::exception) {
        if (evt.get_source_id() != engine::window::get_id()) {
            return;
        }

        if (evt.get_type() == engine::window::events::resized) {
            _width = evt.get_value("width").get<int>();
            _height = evt.get_value("height").get<int>();
        } else
        if (evt.get_type() == engine::window::events::closed) {
            _should_close = true;
        } else {
            TS_ASSERT(false);
        }
    }
};

class WindowTestSuite : public CxxTest::TestSuite {
  public:
    void setUp() {
    }

    void tearDown() {
    }

    void testWindow() {
        engine::engine engine(60);
        std::unique_ptr<engine::window> window_ptr(
            new engine::window("test", json::json({
                {"width", util::value::build<int>(400)},
                {"height", util::value::build<int>(300)},
                {"fullscreen", util::value::build<bool>(false)}
            }))
        );
        window_monitor* monitor = new window_monitor();
        std::unique_ptr<window_monitor> monitor_ptr(monitor);

        window_ptr->register_listener(&engine);
        window_ptr->register_listener(monitor_ptr.get());

        std::thread thread([](engine::window* win) {
            usleep(5000);
            win->resize(1600, 1000);
            win->close();
        }, window_ptr.get());

        engine.plug_module(std::move(window_ptr));
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
        TS_ASSERT(monitor->should_close() == true);
        TS_ASSERT(monitor->get_width() == 1600);
        TS_ASSERT(monitor->get_height() == 1000);
    }

    void testWindowBadConfiguration() {
        engine::engine engine(60);
        std::unique_ptr<engine::window> window_ptr(
            new engine::window("test", json::json({
                {"width", util::value::build<int>(400)},
                {"qshsqiu", util::value::build<int>(300)}
            }))
        );
        engine.plug_module(std::move(window_ptr));

        TS_ASSERT_THROWS(engine.start(), util::exception);
    }

    void testWindowWrongResolution() {
        engine::engine engine(60);
        std::unique_ptr<engine::window> window_ptr(
            new engine::window("test", json::json({
                {"width", util::value::build<int>(123)},
                {"height", util::value::build<int>(456)},
                {"fullscreen", util::value::build<bool>(true)},
            }))
        );
        engine.plug_module(std::move(window_ptr));

        TS_ASSERT_THROWS(engine.start(), util::exception);
    }
};

