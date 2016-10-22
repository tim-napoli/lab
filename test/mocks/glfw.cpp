#include <cstdio>
#include <sys/time.h>
#include "mocks/glfw.hpp"

double _initial_time = 0.0;

bool glfwInit(void) {
    _initial_time = glfwGetTime();
    return true;
}

void glfwTerminate(void) {

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

