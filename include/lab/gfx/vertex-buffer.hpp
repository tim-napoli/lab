/**
 * @module      lab::gfx::vertex_buffer
 * @brief       Vertex buffer class.
 * @author      Timothée Napoli
 * @license     WTFPL
 * @copyright   Timothée Napoli (c) 2017
 */
#ifndef _lab_gfx_vertex_buffer_hpp_
#define _lab_gfx_vertex_buffer_hpp_

#include <vector>
#include <GL/gl.h>
#include <glm/glm.hpp>

namespace lab { namespace gfx {

/**
 * A vertex buffer is a simple object allowing to draw a set of triangles with
 * attributes.
 * A vertex and its attributes must be glm::vec2. When specifiyng vertexes to
 * the buffer, they must be presented as a list such following for a buffer
 * of n vertexes with i attributes:
 * { vertex_1, attrib_1_1, ..., attrib_1_i,
 *   ...,
 *   vertex_n, attrib_n_1, ..., attrib_n_i
 * }
 */
class vertex_buffer {
  public:
    enum type : GLenum {
        /**
         * The vertexes of the vertex buffer will not be updated.
         */
        static_draw = GL_STATIC_DRAW,

        /**
         * The vertexes of the vertex buffer will frequently be updated.
         */
        dynamic_draw = GL_DYNAMIC_DRAW,
    };

  private:
    GLuint  _vbo_id;
    GLuint  _vao_id;
    size_t  _npoints;
    type    _type;
    size_t  _nattribs;

  public:
    vertex_buffer();

    /**
     * Create a new vertex buffer.
     * @param type      The type of the buffer (static or dynamic).
     * @param vertexes  Vertexes and attributes of the buffer (see class
     *                  documentation).
     * @param nattribs  Number of attributes per vertex the buffer has.
     */
    vertex_buffer(type type, const std::vector<glm::vec2>& vertexes,
                  size_t nattribs);

    ~vertex_buffer();

    /**
     * Update the buffer vertexes and attributes.
     * @param vertexes Vertexes and attributes (see class documentation).
     */
    void update_vertexes(const std::vector<glm::vec2>& vertexes);

    /**
     * Draw the vertex buffer to the current rendering target.
     */
    void draw() const;

    /**
     * Destroy the vertexes.
     */
    void destroy();
};

}}

#endif

