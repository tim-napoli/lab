#include <ctime>
#include <cxxtest/TestSuite.h>
#include "test/math/common.hpp"
#include "lab/math/polygon.hpp"

using namespace lab;
using namespace lab::math;
using namespace lab::test;

std::vector<glm::vec2> gen_simple_path(float xmin, float xmax,
                                       float ymin, float ymax,
                                       size_t npoints)
{
    std::vector<glm::vec2> points;
    float angle = 2 * M_PI / npoints;
    glm::mat2 rotation_matrix = glm::mat2(
        cos(angle), -sin(angle),
        sin(angle), cos(angle)
    );

    glm::vec2 base_vector = glm::vec2(1.0, 0.0);
    for (size_t i = 0; i < npoints; i++) {
        glm::vec2 point = base_vector * gen_float(0.01, 1.0);
        points.push_back(point);
        base_vector = rotation_matrix * base_vector;
    }

    return points;
}

class PolygonTestSuite : public CxxTest::TestSuite {
  public:
    void setUp() {
        srand(time(NULL));
        std::cerr << std::endl;
    }

    void tearDown() {

    }

    void testPolygonBuild() {
        for (size_t i = 0; i < 10000; i++) {
            size_t npoints = 3 + rand() % 30;
            std::vector<glm::vec2> path = gen_simple_path(0.0, 1.0, 0.0, 1.0,
                                                          npoints);
            TS_ASSERT_THROWS_NOTHING(polygon::build(path));
        }
    }
};

