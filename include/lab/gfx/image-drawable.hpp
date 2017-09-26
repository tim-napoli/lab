/**
 * @module      lab::gfx::image_drawable
 * @brief       The engine gfx image drawble class.
 * @author      Timothée Napoli
 * @license     WTFPL
 * @copyright   Timothée Napoli (c) 2016
 */
#ifndef _lab_gfx_image_image_hpp_
#define _lab_gfx_image_image_hpp_

#include <vector>
#include <string>
#include "lab/gfx/drawable.hpp"
#include "lab/gfx/image.hpp"

namespace lab { namespace gfx {

/**
 * An image drawable is a drawable that draw an image.
 */
class image_drawable : public drawable {
  private:
    const image* _image;
    std::vector<std::string> _textures_name;

  public:
    image_drawable();

    image_drawable(glm::mat3 model_view_matrix, const image* img,
                   std::vector<std::string> textures_name);

    virtual ~image_drawable();

    void bind(program& prg) const throw(util::exception);

    void draw() const;
};

}}

#endif

