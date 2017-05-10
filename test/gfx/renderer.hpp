#include <unistd.h>
#include <thread>
#include <cxxtest/TestSuite.h>
#include "mocks/gl.hpp"
#include "lab/engine/engine.hpp"
#include "lab/engine/window.hpp"
#include "lab/gfx/renderer.hpp"

using namespace lab;

#define WINDOW_WIDTH    400
#define WINDOW_HEIGHT   300
#define VIRT_WIDTH      800
#define VIRT_HEIGHT     400

void is_valid_real_to_virt_matrix(const glm::mat4 mat) {
    glm::vec4 click((float)(rand() % WINDOW_WIDTH),
                    (float)(rand() % WINDOW_HEIGHT), 0.0f, 1.0f);
    glm::vec4 transposed = mat * click;

    TS_ASSERT(transposed.x == click.x * 0.5);
    TS_ASSERT(transposed.y == (click.y - 50) * 0.5);
};

class RendererTestSuite : public CxxTest::TestSuite {
  public:
    void setUp() {
    }

    void tearDown() {
    }

    void testRenderer() {
        gfx::renderer r(VIRT_WIDTH, VIRT_HEIGHT);

        r.resize(WINDOW_WIDTH, WINDOW_HEIGHT);
        is_valid_real_to_virt_matrix(r.get_real_to_virt_matrix());
    }
};

