#include "lab/math/transform.hpp"

namespace lab { namespace math {

transform::transform()
        : _translation(0.0, 0.0)
        , _scale(1.0, 1.0)
        , _rotation(0.0)
{

}

transform::transform(glm::vec2 translation, glm::vec2 scale, float rotation)
        : _translation(translation)
        , _scale(scale)
        , _rotation(rotation)
{

}

transform::~transform() {

}

glm::vec2 transform::apply(glm::vec2 v) const {
    glm::mat2 rotation_matrix(
        cos(_rotation), -sin(_rotation),
        sin(_rotation), cos(_rotation)
    );

    return v * rotation_matrix * _scale + _translation;
}

glm::mat3 transform::to_matrix() const {
    glm::mat3 rotation_matrix = glm::mat3(
        cos(_rotation), -sin(_rotation), 0.0,
        sin(_rotation),  cos(_rotation), 0.0,
        0.0,            0.0,           1.0
    );
    glm::mat3 scale_matrix = glm::mat3(
        _scale.x, 0.0,      0.0,
        0.0,      _scale.y, 0.0,
        0.0,      0.0,      1.0
    );
    glm::mat3 translation_matrix = glm::mat3(
        1.0, 0.0, _translation.x,
        0.0, 1.0, _translation.y,
        0.0, 0.0, 1.0
    );
    return rotation_matrix * scale_matrix * translation_matrix;
}

}}

