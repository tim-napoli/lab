#include <GL/glew.h>
#include "lab/gfx/vertex-buffer.hpp"

namespace lab { namespace gfx {

vertex_buffer::vertex_buffer()
        : _vbo_id(0)
        , _vao_id(0)
        , _npoints(0)
        , _type(static_draw)
        , _nattribs(0)
{
}

vertex_buffer::~vertex_buffer() {

}

vertex_buffer::vertex_buffer(type type, const std::vector<glm::vec2>& vertexes,
                             size_t nattribs)
    : _vbo_id(0)
    , _vao_id(0)
    , _npoints(vertexes.size())
    , _type(type)
    , _nattribs(nattribs)
{
    glGenBuffers(1, &_vbo_id);
    glGenVertexArrays(1, &_vao_id);
    update_vertexes(vertexes);
}

void vertex_buffer::update_vertexes(const std::vector<glm::vec2>& vertexes) {
    _npoints = vertexes.size() / _nattribs;

    glBindVertexArray(_vao_id);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo_id);
    glBufferData(GL_ARRAY_BUFFER, vertexes.size() * sizeof(glm::vec2),
                 vertexes.data(), _type);

    for (size_t attr = 0; attr < _nattribs; attr++) {
        glVertexAttribPointer(
            attr, 2, GL_FLOAT, GL_FALSE, 2 * _nattribs * sizeof(GLfloat),
            (GLvoid*)(attr * 2 * sizeof(GLfloat))
        );
        glEnableVertexAttribArray(attr);
    }

    glBindVertexArray(0);
}

void vertex_buffer::draw() const {
    glBindVertexArray(_vao_id);
    glDrawArrays(GL_TRIANGLES, 0, _npoints);
    glBindVertexArray(0);
}

void vertex_buffer::destroy() {
    glDeleteVertexArrays(1, &_vao_id);
    glDeleteBuffers(1, &_vbo_id);
}

}}

