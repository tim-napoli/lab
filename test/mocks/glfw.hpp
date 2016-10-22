#ifndef _test_mocks_glfw_hpp_
#define _test_mocks_glfw_hpp_

/* Forbid inclusion of glfw3 */
#define _glfw3_h_

extern "C" {

bool glfwInit(void);

void glfwTerminate(void);

double glfwGetTime(void);

typedef void (*GLFWerrorfun)(int, const char*);

GLFWerrorfun glfwSetErrorCallback(GLFWerrorfun cb);

}

#endif

