#include <unistd.h>
#include <thread>
#include <cxxtest/TestSuite.h>
#include "mocks/gl.hpp"
#include "mocks/glfw.hpp"

#define private public  // Wow, that's tricky ?
#include "lab/engine/engine.hpp"

using namespace lab;

// window monitor -------------------------------------------------------------

class window_monitor : public event::listener {
  private:
    bool _should_close;
    int  _width;
    int  _height;

  public:
    window_monitor() : _should_close(false)
                     , _width(0)
                     , _height(0)
    {

    }

    ~window_monitor() {

    }

    bool should_close() const {return _should_close;}
    int get_width() const {return _width;}
    int get_height() const {return _height;}

    void notify(const event::event& evt) throw(util::exception) {
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

// keyboard monitor -----------------------------------------------------------

class keyboard_monitor : public event::listener {
  private:
    int _pressed_count;
    int _repeated_count;
    int _released_count;

  public:
    keyboard_monitor() : _pressed_count(0)
                       , _repeated_count(0)
                       , _released_count(0)
    {

    }

    ~keyboard_monitor() {

    }

    int get_pressed_count() const {return _pressed_count;}
    int get_repeated_count() const {return _repeated_count;}
    int get_released_count() const {return _released_count;}

    void notify(const event::event& evt) throw(util::exception) {
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

// fake screen ----------------------------------------------------------------

class fake_screen : public engine::screen {
  public:
    void start() throw(util::exception) {

    }

    void update() throw(util::exception) {

    }

    void stop() throw(util::exception) {

    }

    void notify(const event::event& evt) throw(util::exception) {

    }
};

// tests ----------------------------------------------------------------------

class EngineTestSuite : public CxxTest::TestSuite {
  public:
    void setUp() {

    }

    void tearDown() {

    }

    void testEngineTicks() {
        engine::engine engine("test-ticker", 400, 300, false, 60);
        engine.push_screen(new fake_screen());

        try {
            engine.start();

            std::thread thread = std::thread([](engine::engine* e) {
                usleep(10 * (1.0 / 60) * 1E6);
                e->close();
            }, &engine);

            double start_time = glfwGetTime();
            engine.run();
            double stop_time = glfwGetTime();
            TS_ASSERT(stop_time - start_time >= 0.99 * 10 * (1.0 / 60));
            TS_ASSERT(stop_time - start_time <= 1.01 * 10 * (1.0 / 60));

            engine.stop();
            thread.join();
        } catch(util::exception ex) {
            TS_WARN(ex.get_message());
            TS_ASSERT(false);
        }
    }

    void testEngineWindowResize() {
        engine::engine engine("test-window", 400, 300, false, 60);
        engine.push_screen(new fake_screen());
        window_monitor monitor;

        engine._window.register_listener(&monitor);

        std::thread thread = std::thread([](engine::engine* e) {
            usleep(5000);
            e->_window.resize(1600, 1000);
            e->_window.close();
        }, &engine);

        try {
            engine.start();
            engine.run();
            engine.stop();
        } catch(util::exception ex) {
            TS_WARN(ex.get_message());
            TS_ASSERT(false);
        }

        thread.join();
        TS_ASSERT(monitor.should_close());
        TS_ASSERT(monitor.get_width() == 1600);
        TS_ASSERT(monitor.get_height() == 1000);
    }

    void testEngineWindowWrongResolution() {
        engine::engine engine("test-window-wrong-res", 123, 456, true, 60);
        engine.push_screen(new fake_screen());
        TS_ASSERT_THROWS(engine.start(), util::exception);
    }

    void testEngineKeyboard() {
        engine::engine engine("test-keyboard", 400, 300, false, 60);
        engine.push_screen(new fake_screen());
        keyboard_monitor monitor;

        engine._keyboard.register_listener(&monitor);

        std::thread thread = std::thread([](engine::engine* e) {
            usleep(5000);

            glfw_mock_fake_key_action(GLFW_KEY_ENTER, 0, GLFW_PRESS, 0);
            glfw_mock_fake_key_action(GLFW_KEY_ENTER, 0, GLFW_RELEASE, 0);
            glfw_mock_fake_key_action(GLFW_KEY_ENTER, 0, GLFW_REPEAT, 0);
            glfw_mock_fake_key_action(GLFW_KEY_ENTER, 0, GLFW_PRESS, 0);
            glfw_mock_fake_key_action(GLFW_KEY_ENTER, 0, GLFW_RELEASE, 0);
            glfw_mock_fake_key_action(GLFW_KEY_ENTER, 0, GLFW_REPEAT, 0);

            e->_window.close();
        }, &engine);

        try {
            engine.start();
            engine.run();
            engine.stop();
        } catch(util::exception ex) {
            TS_WARN(ex.get_message());
            TS_ASSERT(false);
        }

        thread.join();
        TS_ASSERT(monitor.get_pressed_count() == 2);
        TS_ASSERT(monitor.get_released_count() == 2);
        TS_ASSERT(monitor.get_repeated_count() == 2);
    }

};

// ----------------------------------------------------------------------------

