#include <unistd.h>
#include <cxxtest/TestSuite.h>
#include "mocks/glfw.hpp"

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
};


