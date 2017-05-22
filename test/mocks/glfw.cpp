#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <unistd.h>
#include <sys/time.h>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include "mocks/glfw.hpp"

static double _initial_time = 0.0;
static GLFWmonitor* _monitor = NULL;
static GLFWwindow* _current_context = NULL;
static bool _should_exit = false;

struct _glfw_key_event {
    int key;
    int scancode;
    int action;
    int mode;

    _glfw_key_event(int k, int s, int a, int m) : key(k)
                                                , scancode(s)
                                                , action(a)
                                                , mode(m)
    {
    }
};
static std::mutex                   _key_events_lock;
static std::vector<_glfw_key_event> _key_events;
static std::thread                  _events_thread;

static void _process_key_events(void) {
    _key_events_lock.lock();
    if (_current_context->key_cb) {
        for (auto it = _key_events.begin(); it != _key_events.end(); it++) {
            _current_context->key_cb(_current_context, it->key, it->scancode,
                                     it->action, it->mode);
        }
    }
    _key_events.clear();
    _key_events_lock.unlock();
}

// This function serves to fake event processing done per GLFW for some events.
static void _event_thread_func() {
    while (!_should_exit) {
        if (_current_context) {
            _process_key_events();
        }

        // We do some random automatic flush to be stressful.
        usleep(rand() % 10000);
    }
}

bool glfwInit(void) {
    int seed = time(0);
    std::cout << "GLFW seed: " << seed << std::endl;
    srand(seed);

    _initial_time = glfwGetTime();
    _monitor = NULL;
    _current_context = NULL;
    _key_events = std::vector<_glfw_key_event>();

    _should_exit = false;
    try {
        _events_thread = std::thread(&_event_thread_func);
    } catch (std::exception err) {
        std::cout << "GLFW mock initialization error: " << err.what()
                  << std::endl;
        return false;
    }

    return true;
}

static void _mock_glfw_free_monitor(GLFWmonitor* monitor) {
    free(monitor->modes);
    free(monitor);
}

void glfwTerminate(void) {
    _should_exit = true;
    _events_thread.join();

    if (_monitor) {
        _mock_glfw_free_monitor(_monitor);
        _monitor = NULL;
    }
    _current_context = NULL;
    _initial_time = 0.0;
}

double glfwGetTime(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    double res = tv.tv_sec + tv.tv_usec * 1E-6;
    return res - _initial_time;
}

GLFWerrorfun glfwSetErrorCallback(GLFWerrorfun cb) {
    return cb;
}

GLFWmonitor* glfwGetPrimaryMonitor(void) {
    if (_monitor) {
        return _monitor;
    }

    GLFWmonitor* monitor = (GLFWmonitor*)malloc(sizeof(GLFWmonitor));
    monitor->nmodes = 3;
    monitor->modes = (GLFWvidmode*)malloc(3 * sizeof(GLFWvidmode));
    monitor->modes[0] = (GLFWvidmode){.width = 400,  .height = 300};
    monitor->modes[1] = (GLFWvidmode){.width = 1600, .height = 900};
    monitor->modes[2] = (GLFWvidmode){.width = 1600, .height = 1000};
    _monitor = monitor;

    return _monitor;
}

const GLFWvidmode* glfwGetVideoModes(GLFWmonitor* monitor, int* nmodes) {
    *nmodes = monitor->nmodes;
    return monitor->modes;
}

GLFWwindow* glfwCreateWindow(int width, int height, const char* title,
                             GLFWmonitor* monitor, GLFWwindow* share)
{
    GLFWwindow* window = (GLFWwindow*)malloc(sizeof(GLFWwindow));
    window->width = width;
    window->height = height;
    strcpy(window->name, title);
    window->monitor = monitor;
    window->swap_count = 0;
    window->user_pointer = NULL;
    window->framebuffer_resize_cb = NULL;
    window->key_cb = NULL;
    window->should_close = 0;
    return window;
}

void glfwDestroyWindow(GLFWwindow* window) {
    free(window);
}

void glfwMakeContextCurrent(GLFWwindow* window) {
    _current_context = window;
}

GLFWwindow* glfwGetCurrentContext(void) {
    return _current_context;
}

void glfwSwapBuffers(GLFWwindow* window) {
    window->swap_count++;
}

void glfwSetWindowUserPointer(GLFWwindow* window, void* ptr) {
    window->user_pointer = ptr;
}

void* glfwGetWindowUserPointer(GLFWwindow* window) {
    return window->user_pointer;
}

GLFWframebuffersizefun
glfwSetFramebufferSizeCallback(GLFWwindow* window,
                               GLFWframebuffersizefun callback)
{
    GLFWframebuffersizefun prev = window->framebuffer_resize_cb;
    window->framebuffer_resize_cb = callback;
    return prev;
}

void glfwSetWindowSize(GLFWwindow* window, int width, int height) {
    window->width = width;
    window->height = height;
    if (window->framebuffer_resize_cb != NULL) {
        window->framebuffer_resize_cb(window, width, height);
    }
}

int glfwWindowShouldClose(GLFWwindow* window) {
    return window->should_close;
}

void glfwSetWindowShouldClose(GLFWwindow* window, int should_close) {
    window->should_close = should_close;
}

GLFWkeyfun glfwSetKeyCallback(GLFWwindow* window, GLFWkeyfun callback) {
    GLFWkeyfun prev = window->key_cb;
    window->key_cb = callback;
    return prev;
}

void glfwPollEvents(void) {
    _process_key_events();
}

void glfw_mock_fake_key_action(int key, int scancode, int action, int mods) {
    assert(_current_context != NULL);
    assert(_current_context->key_cb != NULL);
    _key_events_lock.lock();
    _key_events.push_back(_glfw_key_event(key, scancode, action, mods));
    _key_events_lock.unlock();
}

