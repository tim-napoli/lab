#include "lab/engine/keyboard.hpp"
#include "lab/engine/window_user_data.hpp"

namespace lab { namespace engine {

keyboard::keyboard()
{

}

keyboard::~keyboard() {

}

static void _on_key_event(GLFWwindow* window, int key, int scancode,
                          int action, int mods)
{
    window_user_data* win_data =
        (window_user_data*)glfwGetWindowUserPointer(window);
    keyboard* keyb = win_data->keyb;

    event::event::type evt_type = (action == GLFW_PRESS)
                                ? keyboard::events::key_pressed
                                : (action == GLFW_REPEAT)
                                ? keyboard::events::key_repeated
                                : keyboard::events::key_released;
    keyb->send_event(event::event(
        evt_type, {
            {"key", util::value::build<int>(key)},
            {"mods", util::value::build<int>(mods)}
        })
    );
}

void keyboard::start() throw(util::exception) {
    GLFWwindow* window = glfwGetCurrentContext();
    if (window == nullptr) {
        throw util::exception("keyboard module must be started after "
                              "window module.");
    }

    window_user_data* win_data =
        (window_user_data*)glfwGetWindowUserPointer(window);
    win_data->keyb = this;

    glfwSetKeyCallback(window, &_on_key_event);
}

void keyboard::update() throw(util::exception) {

}

void keyboard::stop() throw(util::exception) {
    GLFWwindow* window = glfwGetCurrentContext();
    glfwSetKeyCallback(window, nullptr);
}

}}

