/**
 * @module      lab::gfx::image
 * @brief       The engine gfx image class.
 * @author      Timothée Napoli
 * @license     WTFPL
 * @copyright   Timothée Napoli (c) 2016
 */
#ifndef _lab_gfx_image_hpp_
#define _lab_gfx_image_hpp_

#include <glm/glm.hpp>
#include "lab/gfx/texture.hpp"
#include "lab/gfx/vertex-buffer.hpp"

namespace lab { namespace gfx {

/**
 * An image is a gfx high-level class that allows to draw images to the current
 * rendering target using specific dimensions and anchor point (or hot point).
 *
 * An image is not limited to only one texture. When an image is drawn, all
 * its textures are binded to the OpenGL texture unit corresponding to
 * their index in the image's textures vector.
 */
class image {
  private:
    std::vector<texture> _textures;
    glm::vec2            _hot_point;
    vertex_buffer        _points;

    void do_points(glm::vec2 dimensions);

  public:
    image();

    /**
     * @param texs Textures the image will contain. The textures are now owned
     *             by the image, and will be destroy at the next image's
     *             destroy call.
     * @param hot_point  The image anchor point.
     * @param dimensions The image dimensions.
     */
    image(const std::vector<texture>& texs, glm::vec2 hot_point,
          glm::vec2 dimensions);

    ~image();

    /**
     * Draw the image to the current rendering target.
     */
    void draw() const;

    /**
     * Destroy the image and all its textures.
     */
    void destroy();
};

}}

#endif

