#include <unistd.h>
#include <cxxtest/TestSuite.h>
#include "mocks/gl.hpp"

class GlTestSuite : public CxxTest::TestSuite {
  public:
    void setUp() {
    }

    void tearDown() {
    }

    void testGlViewport() {
        glViewport(2, 3, 5, 7);
        TS_ASSERT(gl_mock_viewport.x == 2);
        TS_ASSERT(gl_mock_viewport.y == 3);
        TS_ASSERT(gl_mock_viewport.width == 5);
        TS_ASSERT(gl_mock_viewport.height == 7);
    }
};


