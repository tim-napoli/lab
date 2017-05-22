/**
 * @module      lab::gfx::framebuffer
 * @brief       The engine gfx framebuffer class.
 * @author      Timothée Napoli
 * @license     WTFPL
 * @copyright   Timothée Napoli (c) 2016
 */
#ifndef _lab_gfx_framebuffer_hpp_
#define _lab_gfx_framebuffer_hpp_

#include <GL/gl.h>
#include "lab/util/exception.hpp"

namespace lab { namespace gfx {

/**
 * A framebuffer is used as another rendering target than the screen.
 * The renderer can choose to switch between rendering framebuffers. Following
 * drawing calls will be printed in the framebuffer's texture.
 *
 * A framebuffer can be created with specific dimensions.
 *
 * When a framebuffer is created, it is associated to an internal texture
 * object.
 */
class framebuffer {
  private:
    GLuint _fbo_id;
    GLuint _texture_id;
    int    _width, _height;

  public:
    framebuffer();
    framebuffer(int width, int height) throw(util::exception);

    ~framebuffer();

    GLuint get_texture_id() const {return _texture_id;}

    GLuint get_fbo_id() const {return _fbo_id;}

    /**
     * Destroy the framebuffer object and its texture.
     */
    void destroy();

    int get_width() const {return _width;}

    int get_height() const {return _height;}

    /**
     * Binds the framebuffer's texture to the specific OpenGL texture unit.
     */
    void use_texture(GLuint unit);
};

}}

#endif

