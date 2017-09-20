/**
 * @module      lab::game::object
 * @brief       Labengine game object.
 * @author      Timothée Napoli
 * @license     WTFPL
 * @copyright   Timothée Napoli (c) 2017
 */

#include <glm/glm.hpp>

namespace lab { namespace math {

/**
 * A geometric 2D transformation defined with a translation vector, a scale
 * vector and a rotation.
 * Applying a transform to an object do operations in this order :
 * 1) rotation
 * 2) translation
 * 3) scaling
 * Notates that the rotation has center (0, 0).
 */
class transform {
  private:
    glm::vec2 _translation;
    glm::vec2 _scale;
    float     _rotation;

  public:
    transform();
    transform(glm::vec2 translation, glm::vec2 scale, float rotation);

    ~transform();

    glm::vec2 get_translation() const {return _translation;}
    glm::vec2 get_scale() const {return _scale;}
    float get_rotation() const {return _rotation;}

    void set_translation(glm::vec2 v) {_translation = v;}
    void set_scale(glm::vec2 v) {_scale = v;}
    void set_rotation(float a) {_rotation = a;}

    /**
     * Transform the given vector using this transform.
     * The vector is rotated, scaled then translated.
     * @code
     *  transform t = transform(
     *      glm::vec2(5.0, 0.0), // Translation
     *      glm::vec2(2.0, 2.0), // Scale
     *      M_PI * 0.5           // Rotation
     *  );
     *  glm::vec2 result = t.transform(glm::vec2(1.0, 2.0));
     *  assert(result == glm::vec2(-2.0 * 2.0 + 5.0, 1.0 * 2.0 + 0.0));
     * @endcode
     */
    glm::vec2 apply(glm::vec2 v) const;

    /**
     * Get a matrix representation of this transformation.
     */
    glm::mat3 to_matrix() const;
};

}}
