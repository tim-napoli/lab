#include <cassert>
#include "lab/gfx/image-drawable.hpp"

namespace lab { namespace gfx {

image_drawable::image_drawable()
        : drawable(glm::mat3())
        , _image(NULL)
        , _textures_name()
{

}

image_drawable::image_drawable(glm::mat3 model_view_matrix,
                               const image* img,
                               std::vector<std::string> textures_name)
        : drawable(model_view_matrix)
        , _image(img)
        , _textures_name(textures_name)
{
    assert(_textures_name.size() == _image->get_textures().size());
}

image_drawable::~image_drawable() {

}

void image_drawable::bind(program& prg) const throw(util::exception) {
    drawable::bind(prg);
    for (size_t i = 0; i < _textures_name.size(); i++) {
        prg.set_uniform_sampler2d(_textures_name[i], i);
    }
}

void image_drawable::draw() const {
    _image->draw();
}

}}

