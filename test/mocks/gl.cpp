#include "mocks/gl.hpp"

gl_mock_viewport_t gl_mock_viewport = {
    .x = -1, .y = -1, .width = -1, .height = -1
};

static int _glew_initialized = 0;

void glViewport(GLint x, GLint y, GLint width, GLint height) {
    gl_mock_viewport.x = x;
    gl_mock_viewport.y = y;
    gl_mock_viewport.width = width;
    gl_mock_viewport.height = height;
}

int glewInit() {
    _glew_initialized = true;
    return 1;
}

