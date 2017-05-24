#include "lab/engine/window.hpp"
#include "lab/engine/window_user_data.hpp"

namespace lab { namespace engine {

window::window(const std::string& name, int width, int height, bool fullscreen)
        : _window(NULL)
        , _name(name)
        , _width(width)
        , _height(height)
        , _fullscreen(fullscreen)
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

void window::set_size(int width, int height) {
    _width = width;
    _height = height;
}

static void _on_framebuffer_resize(GLFWwindow* glfw_win, int width, int height)
{
    window_user_data* win_data =
        (window_user_data*)glfwGetWindowUserPointer(glfw_win);
    window* win = win_data->win;
    win->set_size(width, height);
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
    GLFWmonitor* monitor = (_fullscreen) ? glfwGetPrimaryMonitor() : NULL;
    if (monitor) {
        if (!monitor_is_compatible_size(monitor, _width, _height)) {
            throw util::exception("invalid window size for fullscreen mode");
        }
    }

    // XXX window hints needed ?
    _window = glfwCreateWindow(_width, _height, _name.c_str(), monitor, NULL);
    if (_window == NULL) {
        throw util::exception("cannot create window");
    }
    glfwMakeContextCurrent(_window);

    window_user_data* window_data = new window_user_data;
    window_data->win = this;

    glfwSetWindowUserPointer(_window, window_data);
    glfwSetFramebufferSizeCallback(_window, &_on_framebuffer_resize);

    resize(_width, _height);
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

}}

