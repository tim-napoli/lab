#include <cassert>
#include <algorithm>
#include "lab/gfx/image.hpp"

namespace lab { namespace gfx {

image::image() {

}

image::image(const std::vector<texture>& texs, glm::vec2 hot_point,
             glm::vec2 dimensions)
    : _textures(texs)
    , _hot_point(hot_point)
{
    do_points(dimensions);
}

image::~image() {

}

void image::do_points(glm::vec2 dims) {
    _points = vertex_buffer(
        vertex_buffer::static_draw,
        (std::vector<glm::vec2>) {
            glm::vec2(0.0, 0.0),        glm::vec2(0.0, 0.0),
            glm::vec2(dims.x, 0.0),     glm::vec2(1.0, 0.0),
            glm::vec2(dims.x, dims.y),  glm::vec2(1.0, 1.0),

            glm::vec2(0.0, 0.0),        glm::vec2(0.0, 0.0),
            glm::vec2(0.0, dims.y),     glm::vec2(0.0, 1.0),
            glm::vec2(dims.x, dims.y),  glm::vec2(1.0, 1.0),
    }, 2);
}

void image::draw() const {
    static GLenum texture_units[] = {
        GL_TEXTURE0,
        GL_TEXTURE1,
        GL_TEXTURE2,
        GL_TEXTURE3,
        GL_TEXTURE4,
        GL_TEXTURE5,
        GL_TEXTURE6,
        GL_TEXTURE7,
    };
    // Ensure we don't have to many textures.
    assert(_textures.size() < sizeof(texture_units) / sizeof(GLenum));

    for (size_t i = 0; i < _textures.size(); i++) {
        glActiveTexture(texture_units[i]);
        glBindTexture(GL_TEXTURE_2D, _textures[i].get_texture_id());
    }

    _points.draw();
}

void image::destroy() {
    std::for_each(_textures.begin(), _textures.end(), [](texture& tex) {
        tex.destroy();
    });
    _points.destroy();
}

}}

