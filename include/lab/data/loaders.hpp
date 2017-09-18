/**
 * @module      lab::data::loaders
 * @brief       Data files loaders.
 * @author      Timothée Napoli
 * @license     WTFPL
 * @copyright   Timothée Napoli (c) 2017
 */
#ifndef _lab_data_loaders_hpp_
#define _lab_data_loaders_hpp_

#include <map>
#include "lab/util/exception.hpp"
#include "lab/gfx/texture.hpp"
#include "lab/gfx/image.hpp"
#include "lab/gfx/animation.hpp"

namespace lab { namespace data {

gfx::texture load_texture(const std::string& texture_path)
    throw(util::exception);

gfx::image load_image(const std::map<std::string, gfx::texture>& textures,
                      const std::string& image_path)
    throw(util::exception);

gfx::animation load_animation(const std::map<std::string, gfx::image>& images,
                              const std::string animation_path)
    throw(util::exception);

}}

#endif


