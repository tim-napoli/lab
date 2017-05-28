#include <ctime>
#include <cxxtest/TestSuite.h>
#include "test/math/common.hpp"
#include "lab/math/box.hpp"

using namespace lab;
using namespace lab::math;
using namespace lab::test;

class BoxTestSuite : public CxxTest::TestSuite {
  public:
    void setUp() {
        srand(time(NULL));
        std::cerr << std::endl;
    }

    void tearDown() {

    }

    void testBoxPointIn() {
        for (int i = 0; i < 10000; i++) {
            box b = gen_box(0.0, 1.0, 0.0, 1.0);
            glm::vec2 p = gen_vec2(b.left(), b.right(), b.bottom(), b.top());
            TS_ASSERT(b.contains(p));
        }
    }

    void testBoxContainsBox() {
        for (int i = 0; i < 10000; i++) {
            box b = gen_box(0.0, 1.0, 0.0, 1.0);
            glm::vec2 p = gen_vec2(b.left(), b.right(), b.bottom(), b.top());
            glm::vec2 x = gen_vec2(0.0, 1.0, 0.0, 1.0);
            box bb = box(std::min(p.x, x.x), std::max(p.x, x.x),
                         std::min(p.y, x.y), std::max(p.y, x.y));
            TS_ASSERT(b.intersects(bb));
            TS_ASSERT(bb.intersects(b));
        }
    }
};

