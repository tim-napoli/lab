#include "lab/engine/window.hpp"
#include "lab/engine/window_user_data.hpp"

namespace lab { namespace engine {

window::window(const std::string& name, const json::json& cfg)
        : module("window", cfg)
        , _window(NULL)
        , _name(name)
{

}

window::~window() {

}

bool window::monitor_is_compatible_size(GLFWmonitor* monitor,
                                        int width, int height)
{
    int nmodes = 0;
    const GLFWvidmode* modes = glfwGetVideoModes(monitor, &nmodes);
    if (modes == NULL) {
        return false;
    }
    for (int i = 0; i < nmodes; i++) {
        if (modes[i].width == width && modes[i].height == height) {
            return true;
        }
    }
    return false;
}

static void _on_framebuffer_resize(GLFWwindow* glfw_win, int width, int height)
{
    window_user_data* win_data =
        (window_user_data*)glfwGetWindowUserPointer(glfw_win);
    window* win = win_data->win;
    win->send_event(event::event(
        window::events::resized, {
            {"width", util::value::build<int>(width)},
            {"height", util::value::build<int>(height)}
        })
    );
}

void window::resize(int width, int height) {
    glfwSetWindowSize(_window, width, height);
}

void window::close() {
    glfwSetWindowShouldClose(_window, 1);
}

void window::start() throw(util::exception) {
    int width       = get_cfg()["width"].get_value().get<int>();
    int height      = get_cfg()["height"].get_value().get<int>();
    bool fullscreen = get_cfg()["fullscreen"].get_value().get<bool>();

    GLFWmonitor* monitor = (fullscreen) ? glfwGetPrimaryMonitor() : NULL;
    if (monitor) {
        if (!monitor_is_compatible_size(monitor, width, height)) {
            throw util::exception("invalid window size for fullscreen mode");
        }
    }

    // XXX window hints needed ?
    _window = glfwCreateWindow(width, height, _name.c_str(), monitor, NULL);
    if (_window == NULL) {
        throw util::exception("cannot create window");
    }
    glfwMakeContextCurrent(_window);

    window_user_data* window_data = new window_user_data;
    window_data->win = this;

    glfwSetWindowUserPointer(_window, window_data);
    glfwSetFramebufferSizeCallback(_window, &_on_framebuffer_resize);

    resize(width, height);
}

void window::update() throw(util::exception) {
    if (glfwWindowShouldClose(_window)) {
        send_event(event::event(events::closed));
    }
    glfwSwapBuffers(_window);
    glfwPollEvents();
}

void window::stop() throw(util::exception) {
    if (_window) {
        window_user_data* win_data =
            (window_user_data*)glfwGetWindowUserPointer(_window);
        delete win_data;
        glfwDestroyWindow(_window);
    }
}

void window::notify(const event::event& evt) throw(util::exception) {

}

}}

