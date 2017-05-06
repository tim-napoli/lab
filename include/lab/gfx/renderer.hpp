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
#include "lab/engine/module.hpp"

namespace lab { namespace gfx {

/**
 * The renderer module is in charge of the game rendering to the
 * OpenGL framebuffer.
 *
 * The renderer need a virtual screen dimension at its initialisation
 * ("virtual_width" and "virtual_height" integers).
 * Object rendered using the renderer should use the virtual screen coordinate
 * system.
 * The renderer will do the correct transformation to the screen preserving
 * the virtual screen ratio.
 *
 * When the renderer is resized, it emits an event containing the
 * transformation matrix of a point from the window coordinate system to
 * the virtual coordinate system.
 */
class renderer : public engine::module {
  private:
    glm::mat4 _projection_matrix;
    glm::mat4 _real_to_virt_matrix;

    void resize(int win_width, int win_height);

  public:
    enum events : event::event::type {
        /**
         * Sent when the viewport and projection matrix are updated.
         * @param real_to_virt the address of the matrix used to convert
         *                     coordinates in the window coordinate system to
         *                     the virtual coordinate system.
         */
        resized,
    };

    renderer(const json::json& cfg);

    ~renderer();

    void start() throw(util::exception);

    void update() throw(util::exception);

    void stop() throw(util::exception);

    void notify(const event::event& evt) throw(util::exception);

    SOURCE_MAKE_ID(renderer)
};

}}

#endif

