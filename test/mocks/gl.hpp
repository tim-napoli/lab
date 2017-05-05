#ifndef _test_mocks_gl_hpp_
#define _test_mocks_gl_hpp_

/* Forbid inclusion of OpenGL */
#define __gl_h_

extern "C" {

typedef int GLint;

void glViewport(GLint x, GLint y, GLint width, GLint height);

typedef struct gl_mock_viewport {
    GLint x, y, width, height;
} gl_mock_viewport_t;

extern gl_mock_viewport_t gl_mock_viewport;

}

#endif

