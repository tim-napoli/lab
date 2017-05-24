#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "lab/gfx/renderer.hpp"
#include "lab/engine/window.hpp"

namespace lab { namespace gfx {

renderer::renderer(int virtual_width, int virtual_height)
    : _virtual_width(virtual_width)
    , _virtual_height(virtual_height)
{

}

renderer::~renderer() {

}

void renderer::resize(int win_width, int win_height) {
    glm::vec2 virt_dims(_virtual_width, _virtual_height);
    glm::vec2 real_dims(win_width, win_height);

    glm::vec2 ratios = real_dims / virt_dims;
    float ratio = (ratios.x * virt_dims.y > real_dims.y)
                ? ratios.y : ratios.x;

    glm::vec2 transposed_dims = virt_dims * ratio;
    glm::vec2 top_left = (real_dims - transposed_dims) / 2.0f;

    glViewport(top_left.x, top_left.y, transposed_dims.x,
               transposed_dims.y);

    _projection_matrix = glm::ortho(0.0f, transposed_dims.x,
                                    0.0f, transposed_dims.y);

    _real_to_virt_matrix =
        glm::scale(glm::mat4(1.0), glm::vec3(ratio, ratio, 0.0))
      * glm::translate(glm::mat4(1.0), glm::vec3(-top_left.x, -top_left.y, 0.0));
}

}}

