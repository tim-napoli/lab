#include <cstdio>
#include <unistd.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "lab/engine/engine.hpp"

namespace lab { namespace engine {

engine::engine(const std::string& game_name,
               int window_width, int window_height, bool fullscreen,
               int ticks_per_second)
        : event::listener()
        , _exit(false)
        , _refresh_rate(1.0 / ticks_per_second)
        , _window(game_name, window_width, window_height, fullscreen)
        , _keyboard()
        , _screen_manager()
{

}

engine::~engine() {

}

void engine::run() throw(util::exception) {
    while (!_exit && _screen_manager.is_screen()) {
        double current_time = glfwGetTime();

        _keyboard.update();
        _window.update();
        _screen_manager.update(this);

        // Sleeping
        double running_time = glfwGetTime() - current_time;
        double sleep_time = _refresh_rate - running_time;
        if (sleep_time < 0) {
            fprintf(stderr, "skipping %f ticks\n",
                    _refresh_rate / (-sleep_time));
        } else {
            usleep(sleep_time * 1E6);
        }
    }
}

static void error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

void engine::start() throw(util::exception) {
    if (!glfwInit()) {
        throw util::exception("Cannot start GLFW");
    }
    glfwSetErrorCallback(&error_callback);

    _window.register_listener(this);
    _window.register_listener(&_screen_manager);
    _keyboard.register_listener(this);
    _keyboard.register_listener(&_screen_manager);

    try {
        _window.start();
        _keyboard.start();
    } catch (util::exception ex) {
        // We terminate GLFW in case the process will try to restart an
        // engine (like tests).
        glfwTerminate();
        throw ex;
    }

    if (glewInit() != GLEW_OK) {
        throw util::exception("Cannot start GLEW");
    }

}

void engine::stop() throw(util::exception) {
    glfwTerminate();
    // TODO gently stop of the screen manager.
}

void engine::start_screen(std::unique_ptr<screen> scr) {
    _screen_manager.push_screen(std::move(scr));
}

void engine::close_screen() {
    _screen_manager.pop_screen(this);
}

void engine::close() {
    _exit = true;
}

void engine::notify(const event::event& evt) throw(util::exception) {
    if (evt.get_source_id() == window::get_id()) {
        if (evt.get_type() == window::events::closed) {
            close();
        }
    }
}

}}

