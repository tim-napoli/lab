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


typedef struct GLFWvidmode {
    int width;
    int height;
} GLFWvidmode;

typedef struct GLFWmonitor {
    int nmodes;
    GLFWvidmode* modes;
} GLFWmonitor;

GLFWmonitor* glfwGetPrimaryMonitor(void);

/**
 * In the mock library, this function will always return following array
 * (width, height) :
 * {{400, 300}, {1600, 900}, {1600, 1000}}
 */
const GLFWvidmode* glfwGetVideoModes(GLFWmonitor* monitor, int* nmodes);

typedef struct GLFWwindow GLFWwindow;

typedef void (*GLFWframebuffersizefun)(GLFWwindow*, int, int);

struct GLFWwindow {
    char                   name[128];
    int                    width;
    int                    height;
    GLFWmonitor*           monitor;
    size_t                 swap_count;
    void*                  user_pointer;
    GLFWframebuffersizefun framebuffer_resize_cb;
    int                    should_close;
};

GLFWwindow* glfwCreateWindow(int width, int height, const char* title,
                             GLFWmonitor* monitor, GLFWwindow* share);

void glfwDestroyWindow(GLFWwindow* window);

void glfwMakeContextCurrent(GLFWwindow* window);

GLFWwindow* glfwGetCurrentContext(void);

void glfwSwapBuffers(GLFWwindow* window);

void glfwSetWindowUserPointer(GLFWwindow* window, void* ptr);

void* glfwGetWindowUserPointer(GLFWwindow* window);

GLFWframebuffersizefun
glfwSetFramebufferSizeCallback(GLFWwindow* window,
                               GLFWframebuffersizefun callback);

void glfwSetWindowSize(GLFWwindow* window, int width, int height);

int glfwWindowShouldClose(GLFWwindow* window);

void glfwSetWindowShouldClose(GLFWwindow* window, int should_close);

}

#endif

