/**
 * @module      lab::gfx::renderer
 * @brief       The Renderer module.
 * @author      Timothée Napoli
 * @license     WTFPL
 * @copyright   Timothée Napoli (c) 2017
 */
#ifndef _lab_gfx_renderer_hpp_
#define _lab_gfx_renderer_hpp_

#include <glm/mat4x4.hpp>
#include "lab/event/listener.hpp"

namespace lab { namespace gfx {

/**
 * The renderer module is in charge of the game rendering to the
 * OpenGL framebuffer.
 *
 * The renderer need a virtual screen dimension at its initialisation.
 * Object rendered using the renderer should use the virtual screen coordinate
 * system.
 * The renderer will do the correct transformation to the screen preserving
 * the virtual screen ratio.
 */
class renderer {
  private:
    int _virtual_width, _virtual_height;
    glm::mat4 _projection_matrix;
    glm::mat4 _real_to_virt_matrix;


  public:
    renderer(int virtual_width, int virtual_height);

    ~renderer();

    void resize(int win_width, int win_height);
};

}}

#endif

