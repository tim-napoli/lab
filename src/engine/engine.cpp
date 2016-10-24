#include <cstdio>
#include <unistd.h>
#include <GLFW/glfw3.h>
#include "lab/engine/engine.hpp"

namespace lab { namespace engine {

engine::engine(int ticks_per_second)
        : event::listener()
        , event::source()
        , _exit(false)
        , _refresh_rate(1.0 / ticks_per_second)
        , _modules()
{

}

engine::~engine() {

}

void engine::plug_module(std::unique_ptr<module> module) {
    // XXX This is not really RAII but it's safe since an event source
    //     don't own the registred listener.
    //     However, we should think about making modules shared_ptr
    //     instead of unique_ptr...
    register_listener(module.get());
    module->register_listener(this);
    _modules.push_back(std::move(module));
}

void engine::start_modules() throw(util::exception) {
    for (auto it = _modules.begin(); it != _modules.end(); it++) {
        try {
            (*it)->start();
        } catch (util::exception ex) {
            throw util::exception::build_formatted(
                "Engine cannot start module '", (*it)->get_name(), "': ",
                ex.get_message()
            );
        }
    }
}

void engine::update_modules() throw(util::exception) {
    for (auto it = _modules.begin(); it != _modules.end(); it++) {
        try {
            (*it)->update();
        } catch (util::exception ex) {
            throw util::exception::build_formatted(
                "Engine cannot update module '", (*it)->get_name(), "': ",
                ex.get_message()
            );
        }
    }
}

void engine::run() throw(util::exception) {
    while (!_exit) {
        double current_time = glfwGetTime();

        update_modules();

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

    start_modules();
}

void engine::stop_modules() throw(util::exception) {
    for (auto it = _modules.begin(); it != _modules.end(); it++) {
        try {
            (*it)->stop();
        } catch (util::exception ex) {
            throw util::exception::build_formatted(
                "Engine cannot stop module '", (*it)->get_name(), "': ",
                ex.get_message()
            );
        }
    }
}

void engine::stop() throw(util::exception) {
    stop_modules();
    glfwTerminate();
}

void engine::close() {
    _exit = true;
}

void engine::notify(const event::event& evt) throw(util::exception) {
}

}}

