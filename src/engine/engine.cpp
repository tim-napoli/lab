#include <cstdio>
#include <unistd.h>
#include <GLFW/glfw3.h>
#include "lab/engine/engine.hpp"
#include "lab/engine/window.hpp"

namespace lab { namespace engine {

engine::engine(int ticks_per_second)
        : event::listener()
        , event::source()
        , _exit(false)
        , _refresh_rate(1.0 / ticks_per_second)
        , _module_manager()
{

}

engine::~engine() {

}


void engine::run() throw(util::exception) {
    while (!_exit) {
        double current_time = glfwGetTime();

        _module_manager.update();

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

    try {
        _module_manager.register_listener(this);
        _module_manager.start();
    } catch (util::exception ex) {
        // We terminate GLFW in case the process will try to restart an
        // engine (like tests).
        glfwTerminate();
        throw ex;
    }
}

void engine::stop() throw(util::exception) {
    _module_manager.stop();
    glfwTerminate();
}

void engine::close() {
    _exit = true;
}

void engine::plug_module(std::unique_ptr<module> module) {
    _module_manager.plug_module(std::move(module));
}

void engine::notify(const event::event& evt) throw(util::exception) {
    if (evt.get_source_id() == module_manager::get_id()) {
        if (evt.get_type() == window::events::closed) {
            close();
        }
    }
}

}}

