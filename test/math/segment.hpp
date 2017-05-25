#include <ctime>
#include <cxxtest/TestSuite.h>
#include "test/math/common.hpp"
#include "lab/math/segment.hpp"

using namespace lab;
using namespace lab::math;
using namespace lab::test;

class SegmentTestSuite : public CxxTest::TestSuite {
  public:
    void setUp() {
        srand(time(NULL));
        std::cerr << std::endl;
    }

    void tearDown() {

    }

    void SegmentIntersection() {
        segment a = segment(glm::vec2(0.0, 0.0), glm::vec2(1.0, 1.0));
        segment b = segment(glm::vec2(1.0, 0.0), glm::vec2(0.0, 1.0));

        util::optional<glm::vec2> res = a.get_intersection(b);
        TS_ASSERT(res.is_set());
        TS_ASSERT(res.get_value() == glm::vec2(0.5, 0.5));
    }

    void testSegmentIntersection2() {
        segment a = segment(glm::vec2(0.0722, 0), glm::vec2(0.410293, -0.344277));
        segment b = segment(glm::vec2(0.115216, -0.65342), glm::vec2(-0.1145, -0.19832));

        auto res = a.get_intersection(b);
        TS_ASSERT(!res.is_set());
    }

    void testSegmentIntersectionRndCrossing() {
        for (int k = 0; k < 1000000; k++) {
            glm::vec2 i = gen_vec2(0.0, 1.0, 0.0, 1.0);
            glm::vec2 p1 = gen_vec2(0.0, 1.0, 0.0, 1.0);
            glm::vec2 q1 = gen_vec2(0.0, 1.0, 0.0, 1.0);
            glm::vec2 p2 = i + (i - p1) * gen_float(0.0, 1.0);
            glm::vec2 q2 = i + (i - q1) * gen_float(0.0, 1.0);

            segment a = segment(p1, p2);
            segment b = segment(q1, q2);
            glm::vec2 s = a.get_direction();
            glm::vec2 r = b.get_direction();

            util::optional<glm::vec2> res = a.get_intersection(b);
            if (!res.is_set()) {
                std::cerr << "i : " << i << std::endl
                          << "p1: " << p1 << std::endl
                          << "p2: " << p2 << std::endl
                          << "q1: " << q1 << std::endl
                          << "q2: " << q2 << std::endl;
            }
            if (float_are_equal(s.x * r.y - s.y * r.x, 0.0)) {
                TS_ASSERT(res.is_set());
                TS_ASSERT(   vec2_are_equal(res.get_value(), q1)
                          || vec2_are_equal(res.get_value(), q2)
                          || vec2_are_equal(res.get_value(), p1)
                          || vec2_are_equal(res.get_value(), p2));
            } else {
                TS_ASSERT(res.is_set());
                TS_ASSERT(vec2_are_equal(res.get_value(), i));
            }
        }
    }

    void testSegmentIntersectionRndNotCrossing() {
        
    }
};

