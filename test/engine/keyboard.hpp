#include <unistd.h>
#include <thread>
#include <cxxtest/TestSuite.h>
#include "mocks/gl.hpp"
#include "mocks/glfw.hpp"
#include "lab/engine/engine.hpp"
#include "lab/engine/window.hpp"
#include "lab/engine/keyboard.hpp"

using namespace lab;

class keyboard_monitor : public engine::module {
  private:
    int _pressed_count;
    int _repeated_count;
    int _released_count;

  public:
    keyboard_monitor() : engine::module("keyboard_monitor", json::json())
                       , _pressed_count(0)
                       , _repeated_count(0)
                       , _released_count(0)
    {

    }

    ~keyboard_monitor() {

    }

    int get_pressed_count() const {return _pressed_count;}
    int get_repeated_count() const {return _repeated_count;}
    int get_released_count() const {return _released_count;}

    void start() throw(util::exception) {

    }

    void stop() throw(util::exception) {

    }

    void update() throw(util::exception) {

    }

    void notify(const event::event& evt) throw(util::exception) {
        if (evt.get_source_id() != engine::keyboard::get_id()) {
            return;
        }

        switch (evt.get_type()) {
          case engine::keyboard::events::key_pressed:
            _pressed_count++;
            break;

          case engine::keyboard::events::key_repeated:
            _repeated_count++;
            break;

          case engine::keyboard::events::key_released:
            _released_count++;
            break;

          default:
            TS_ASSERT(false);
        }
    }
};

class KeyboardTestSuite : public CxxTest::TestSuite {
  public:
    void setUp() {
    }

    void tearDown() {
    }

    void testKeyboard() {
        engine::engine engine(60);
        std::unique_ptr<engine::window> window_ptr(
            new engine::window("test", json::json({
                {"width", util::value::build<int>(400)},
                {"height", util::value::build<int>(300)},
                {"fullscreen", util::value::build<bool>(false)}
            }))
        );

        std::unique_ptr<engine::keyboard> keyboard_ptr(new engine::keyboard());

        keyboard_monitor* monitor = new keyboard_monitor();
        std::unique_ptr<keyboard_monitor> monitor_ptr(monitor);

        window_ptr->register_listener(&engine);
        keyboard_ptr->register_listener(monitor_ptr.get());

        std::thread thread([](engine::window* win) {
            usleep(5000),

            glfw_mock_fake_key_action(GLFW_KEY_ENTER, 0, GLFW_PRESS, 0);
            glfw_mock_fake_key_action(GLFW_KEY_ENTER, 0, GLFW_RELEASE, 0);
            glfw_mock_fake_key_action(GLFW_KEY_ENTER, 0, GLFW_REPEAT, 0);
            glfw_mock_fake_key_action(GLFW_KEY_ENTER, 0, GLFW_PRESS, 0);
            glfw_mock_fake_key_action(GLFW_KEY_ENTER, 0, GLFW_RELEASE, 0);
            glfw_mock_fake_key_action(GLFW_KEY_ENTER, 0, GLFW_REPEAT, 0);

            win->close();
        }, window_ptr.get());

        engine.plug_module(std::move(window_ptr));
        engine.plug_module(std::move(keyboard_ptr));
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

        TS_ASSERT(monitor->get_pressed_count() == 2);
        TS_ASSERT(monitor->get_released_count() == 2);
        TS_ASSERT(monitor->get_repeated_count() == 2);
    }

    void testKeyboardWithoutWindow() {
        engine::engine engine(60);
        std::unique_ptr<engine::keyboard> keyboard_ptr(new engine::keyboard());

        engine.plug_module(std::move(keyboard_ptr));

        TS_ASSERT_THROWS(engine.start(), util::exception);
    }

    void testKeyboardBeforeWindow() {
        engine::engine engine(60);
        std::unique_ptr<engine::window> window_ptr(
            new engine::window("test", json::json({
                {"width", util::value::build<int>(400)},
                {"height", util::value::build<int>(300)},
                {"fullscreen", util::value::build<bool>(false)}
            }))
        );
        std::unique_ptr<engine::keyboard> keyboard_ptr(new engine::keyboard());

        engine.plug_module(std::move(keyboard_ptr));
        engine.plug_module(std::move(window_ptr));

        TS_ASSERT_THROWS(engine.start(), util::exception);
    }
};

