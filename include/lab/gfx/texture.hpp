/**
 * @module      lab::gfx::texture
 * @brief       The engine gfx texture class.
 * @author      Timothée Napoli
 * @license     WTFPL
 * @copyright   Timothée Napoli (c) 2016
 */
#ifndef _lab_gfx_texture_hpp_
#define _lab_gfx_texture_hpp_

#include <GL/gl.h>
#include <glm/glm.hpp>
#include "lab/util/exception.hpp"

namespace lab { namespace gfx {

/**
 * The texture class allows to load an image file and to store it in the
 * video memory.
 */
class texture {
  private:
    GLuint _texture_id;

  public:
    texture();

    /**
     * Create a texture object using an OpenGL texture id.
     * The texture is now owned by the texture object.
     */
    texture(GLuint tex_id);

    ~texture();

    GLuint get_texture_id() const {return _texture_id;}

    glm::vec2 get_dimensions() const;

    /**
     * Destroy the texture from the video memory.
     */
    void destroy();

    /**
     * Load a texture from the given file. The file format is
     * automatically determined.
     */
    static texture load(const std::string& path) throw(util::exception);
};

}}

#endif

