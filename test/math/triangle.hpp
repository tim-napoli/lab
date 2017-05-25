#include <ctime>
#include <cxxtest/TestSuite.h>
#include "test/math/common.hpp"
#include "lab/math/triangle.hpp"

using namespace lab;
using namespace lab::math;
using namespace lab::test;

class TriangleTestSuite : public CxxTest::TestSuite {
  public:
    void setUp() {
        srand(time(NULL));
        std::cerr << std::endl;
    }

    void tearDown() {

    }

    void testTrianglePointIn() {
        for (int i = 0; i < 100000; i++) {
            glm::vec2 a = gen_vec2(0.0, 1.0, 0.0, 1.0),
                      b = gen_vec2(0.0, 1.0, 0.0, 1.0),
                      c = gen_vec2(0.0, 1.0, 0.0, 1.0);
            glm::vec2 r = b - a,
                      s = c - a;
            glm::vec2 v = r * gen_float(0.0, 1.0)
                        + s * gen_float(0.0, 1.0);
            glm::vec2 p = a + v * 0.5f;
            triangle t = triangle(a, b, c);
            TS_ASSERT(t.contains(p));
        }
    }
};

