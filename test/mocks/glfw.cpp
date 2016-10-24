#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/time.h>
#include "mocks/glfw.hpp"

double _initial_time = 0.0;

static GLFWmonitor* _monitor = NULL;

static GLFWwindow* _current_context = NULL;

bool glfwInit(void) {
    _initial_time = glfwGetTime();
    _monitor = NULL;
    _current_context = NULL;
    return true;
}

static void _mock_glfw_free_monitor(GLFWmonitor* monitor) {
    free(monitor->modes);
    free(monitor);
}

void glfwTerminate(void) {
    if (_monitor) {
        _mock_glfw_free_monitor(_monitor);
        _monitor = NULL;
    }
    _current_context = NULL;
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

