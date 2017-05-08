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
        , _screens_stack()
        , _next_screen(NULL)
        , _close_screen(false)
{

}

engine::~engine() {
    for (auto it = _screens_stack.begin(); it != _screens_stack.end(); it++) {
        delete *it;
    }
    _screens_stack.clear();
}

void engine::close_top_screen() throw(util::exception) {
    screen* scr = top_screen();
    if (scr == NULL) {
        throw util::exception(
            "attempting to close a screen, but no screen left in the screen "
            "stack."
        );
    }

    _screens_stack.pop_back();
    _window.unregister_listener(scr);
    _keyboard.unregister_listener(scr);
    delete(scr);
}

void engine::push_screen(screen* scr) {
    _screens_stack.push_back(scr);
    _window.register_listener(scr);
    _keyboard.register_listener(scr);
}

void engine::run() throw(util::exception) {
    while (!_exit && _screens_stack.size() > 0) {
        double current_time = glfwGetTime();

        // Screen stack management.
        if (_close_screen) {
            close_top_screen();
            if (_screens_stack.size() == 0 && !_next_screen) {
                break;
            }
        }
        if (_next_screen != NULL) {
            push_screen(_next_screen);
            _next_screen = NULL;
        }

        _keyboard.update();
        _window.update();
        top_screen()->update();

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

    if (!glewInit()) {
        throw util::exception("Cannot start GLEW");
    }

    _window.register_listener(this);
    _keyboard.register_listener(this);

    try {
        _window.start();
        _keyboard.start();
    } catch (util::exception ex) {
        // We terminate GLFW in case the process will try to restart an
        // engine (like tests).
        glfwTerminate();
        throw ex;
    }
}

void engine::stop() throw(util::exception) {
    glfwTerminate();
}

void engine::close() {
    _exit = true;
}

void engine::notify(const event::event& evt) throw(util::exception) {
    if (evt.get_source_id() == window::get_id()) {
        if (evt.get_type() == window::events::closed) {
            close();
        }
    } else
    if (evt.get_source_id() == screen::get_id()) {
        if (evt.get_type() == screen::events::start_screen) {
            _next_screen = evt.get_value("screen").get_raw<screen*>();
        } else
        if (evt.get_type() == screen::events::close) {
            _close_screen = true;
        }
    }
}

}}

