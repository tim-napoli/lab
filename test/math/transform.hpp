#include <ctime>
#include <cxxtest/TestSuite.h>
#include "test/math/common.hpp"
#include "lab/math/common.hpp"
#include "lab/math/transform.hpp"

using namespace lab;
using namespace lab::math;
using namespace lab::test;

class TransformTestSuite : public CxxTest::TestSuite {
  public:
    void setUp() {
        srand(time(NULL));
        std::cerr << std::endl;
    }

    void tearDown() {

    }

    glm::mat3 transformation_matrix(glm::vec2 translation, glm::vec2 scale,
                                    float rotation)
    {
        glm::mat3 rotation_matrix = glm::mat3(
            cos(rotation), -sin(rotation), 0.0,
            sin(rotation),  cos(rotation), 0.0,
            0.0,            0.0,           1.0
        );
        glm::mat3 scale_matrix = glm::mat3(
            scale.x, 0.0,     0.0,
            0.0,     scale.y, 0.0,
            0.0,     0.0,     1.0
        );
        glm::mat3 translation_matrix = glm::mat3(
            1.0, 0.0, translation.x,
            0.0, 1.0, translation.y,
            0.0, 0.0, 1.0
        );

        return rotation_matrix * scale_matrix * translation_matrix;
    }

    void testMatrixBuild() {
        glm::mat3 matrix = transformation_matrix(
            glm::vec2(5.0, 1.0),
            glm::vec2(2.0, 2.0),
            M_PI * 0.5
        );

        glm::vec3 v = glm::vec3(2.0, 1.0, 1.0);
        glm::vec3 r = v * matrix;
        TS_ASSERT(float_are_equal(r.x, -1.0 * 2.0 + 5.0));
        TS_ASSERT(float_are_equal(r.y, 2.0 * 2.0 + 1.0));
    }

    void testSimpleTransformApply() {
        transform t = transform(
            glm::vec2(5.0, 1.0),
            glm::vec2(2.0, 2.0),
            M_PI * 0.5
        );
        glm::vec2 v = glm::vec2(2.0, 1.0);
        glm::vec2 r = t.apply(v);
        TS_ASSERT(float_are_equal(r.x, -1.0 * 2.0 + 5.0));
        TS_ASSERT(float_are_equal(r.y, 2.0 * 2.0 + 1.0));
    }

    void testTransformApply() {
        for (int i = 0; i < 10000; i++) {
            glm::vec2 translation = gen_vec2(0.0, 1.0, 0.0, 1.0);
            glm::vec2 scale = gen_vec2(0.0, 1.0, 0.0, 1.0);
            float rotation = gen_float(0.0, M_PI * 2.0);

            glm::mat3 matrix = transformation_matrix(
                translation, scale, rotation
            );
            transform t = transform(translation, scale, rotation);

            glm::vec2 v = gen_vec2(0.0, 1.0, 0.0, 1.0);
            glm::vec3 v_mat = glm::vec3(v, 1.0);
            glm::vec3 matrix_result3 = v_mat * matrix;
            glm::vec2 matrix_result = glm::vec2(
                matrix_result3.x, matrix_result3.y
            );
            glm::vec2 transform_result = t.apply(v);
            TS_ASSERT(vec2_are_equal(transform_result, matrix_result));
        }
    }
};

