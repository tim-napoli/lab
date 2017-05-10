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
#include "lab/gfx/framebuffer.hpp"

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
    glm::vec2 _win_dimensions;


  public:
    renderer(int virtual_width, int virtual_height);

    ~renderer();

    void resize(int win_width, int win_height);

    /**
     * Returns the matrix used to convert coordinates in the window
     * system coordinates to the virtual screen coordinates.
     */
    const glm::mat4& get_real_to_virt_matrix() const {
        return _real_to_virt_matrix;
    }

    /**
     * Returns a pointer to the projection matrix.
     */
    const glm::mat4* get_projection_matrix_ptr() const {
        return &_projection_matrix;
    }

    /**
     * Set the given framebuffer as rendering target.
     */
    void use_framebuffer(const framebuffer& fb);

    /**
     * Set the screen as rendering target.
     */
    void use_screen();

    void set_windows_dimensions(int width, int height) {
        _win_dimensions = glm::vec2(width, height);
    }
};

}}

#endif

