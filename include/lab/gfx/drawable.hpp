/**
 * @module      lab::gfx::drawable
 * @brief       The rendering drawable module.
 * @author      Timothée Napoli
 * @license     WTFPL
 * @copyright   Timothée Napoli (c) 2017
 */
#ifndef _lab_gfx_drawable_hpp_
#define _lab_gfx_drawable_hpp_

#include <glm/glm.hpp>
#include "lab/util/exception.hpp"
#include "lab/gfx/program.hpp"

namespace lab { namespace gfx {

/**
 * A drawable is an object that can be given to a draw renderer pass.
 * A drawable has ever a model-view matrix. It is binded to the shader
 * "model_view_matrix" mat3 uniform using the drawable::bind method.
 */
class drawable {
  private:
    glm::mat3 _model_view_matrix;

  public:
    drawable(glm::mat3 model_view_matrix);

    virtual ~drawable();

    void set_model_view_matrix(glm::mat3 mv_matrix) {
        _model_view_matrix = mv_matrix;
    }

    /**
     * Bind the drawable data on a shader program. When writing your
     * own drawables, you can overload this method to bind your specific
     * drawable data to the shader program.
     * @throw if some data cannot be bound to the shader's uniforms.
     */
    virtual void bind(program& shader) const throw(util::exception);

    virtual void draw() const = 0;
};

}}

#endif

