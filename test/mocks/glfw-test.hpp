#include <unistd.h>
#include <cxxtest/TestSuite.h>
#include "mocks/glfw.hpp"

// Used to test glfw window resize function
static bool window_resized = false;
static void on_window_resize(GLFWwindow* window, int width, int height) {
    window_resized = true;
}

// Used to test glfw keyboard function
static int keyboard_nevents = 0;
static void on_keyboard_key(GLFWwindow* window, int key, int scancode,
                            int action, int mods)
{
    keyboard_nevents++;
}

class GlfwTestSuite : public CxxTest::TestSuite {
  public:
    void setUp() {
        glfwInit();
    }

    void tearDown() {
        glfwTerminate();
    }

    void testGetTime() {
        double start_time = glfwGetTime();
        usleep(1000);
        double stop_time = glfwGetTime();

        // Yes, we have a big imprecision...
        TS_ASSERT(   stop_time - start_time >= 850E-6
                  && stop_time - start_time <= 1150E-6);
    }

    void testGetPrimaryMonitor() {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        TS_ASSERT(monitor->nmodes == 3);
        TS_ASSERT(monitor->modes[0].width  == 400);
        TS_ASSERT(monitor->modes[0].height == 300);
        TS_ASSERT(monitor->modes[1].width  == 1600);
        TS_ASSERT(monitor->modes[1].height == 900);
        TS_ASSERT(monitor->modes[2].width  == 1600);
        TS_ASSERT(monitor->modes[2].height == 1000);
    }

    void testCreateWindow() {
        GLFWwindow* window = glfwCreateWindow(400, 300, "test", NULL, NULL);
        TS_ASSERT(window != NULL);
        TS_ASSERT(window->width  == 400);
        TS_ASSERT(window->height == 300);
        TS_ASSERT(strcmp(window->name, "test") == 0);
        TS_ASSERT(window->monitor == NULL);
        TS_ASSERT(window->swap_count == 0);
        TS_ASSERT(window->user_pointer == NULL);
        TS_ASSERT(window->framebuffer_resize_cb == NULL);
        TS_ASSERT(window->should_close == 0);

        glfwMakeContextCurrent(window);
        TS_ASSERT(glfwGetCurrentContext() == window);

        glfwSwapBuffers(window);
        TS_ASSERT(window->swap_count == 1);

        glfwSetWindowUserPointer(window, window);
        TS_ASSERT(glfwGetWindowUserPointer(window) == window);

        glfwSetFramebufferSizeCallback(window, &on_window_resize);
        window_resized = false;
        glfwSetWindowSize(window, 1600, 900);
        TS_ASSERT(window_resized == true);
        TS_ASSERT(window->width == 1600);
        TS_ASSERT(window->height == 900);

        glfwSetWindowShouldClose(window, 1);
        TS_ASSERT(glfwWindowShouldClose(window) == 1);

        glfwDestroyWindow(window);
    }

    void testKeyboard() {
        GLFWwindow* window = glfwCreateWindow(400, 300, "keyboard", NULL,
                                              NULL);
        glfwMakeContextCurrent(window);
        glfwSetKeyCallback(window, &on_keyboard_key);

        glfw_mock_fake_key_action(GLFW_KEY_ENTER, 0, GLFW_PRESS,
                                  GLFW_MOD_SHIFT);
        glfw_mock_fake_key_action(GLFW_KEY_ENTER, 0, GLFW_RELEASE,
                                  GLFW_MOD_SHIFT);
        glfw_mock_fake_key_action(GLFW_KEY_SPACE, 0, GLFW_PRESS,
                                  GLFW_MOD_ALT);
        glfwPollEvents();
        // Both call to ensure that one call make the events queue empty.
        glfwPollEvents();

        TS_ASSERT(keyboard_nevents == 3);

        glfwDestroyWindow(window);
    }
};


