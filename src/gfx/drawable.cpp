#include "lab/gfx/drawable.hpp"

namespace lab { namespace gfx {

drawable::drawable(glm::mat3 model_view_matrix)
        : _model_view_matrix(model_view_matrix)
{

}

drawable::~drawable() {

}

void drawable::bind(program& shader) const
        throw(util::exception)
{
    shader.set_uniform_mat3("model_view_matrix", &_model_view_matrix);
}

}}
