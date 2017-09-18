/**
 * @module      lab::data::manifest
 * @brief       The data manifest file handling.
 * @author      Timothée Napoli
 * @license     WTFPL
 * @copyright   Timothée Napoli (c) 2017
 */
#ifndef _lab_data_manifest_hpp_
#define _lab_data_manifest_hpp_

#include <map>
#include <string>

#include "lab/util/exception.hpp"
#include "lab/gfx/texture.hpp"
#include "lab/gfx/image.hpp"
#include "lab/gfx/animation.hpp"

namespace lab { namespace data {

/**
 * The manifest class is used to inventory every data a project is using.
 * The manifest should be the only way to access data. This allows to limitate
 * memory usage and to ease memory management of data.
 *
 * You shouldn't write a manifest with you hand, instead, uses the
 * lab-data-editor in the "tools" directory.
 */
class manifest {
  private:
    std::map<std::string, gfx::texture> _textures;
    std::map<std::string, gfx::image> _images;
    std::map<std::string, gfx::animation> _animations;
  public:
    manifest();

    ~manifest();

    gfx::texture get_texture(const std::string& texture) const
        throw(util::exception);

    gfx::image get_image(const std::string& image) const
        throw(util::exception);

    gfx::animation get_animation(const std::string& animation) const
        throw(util::exception);

    void destroy();

    static manifest load(const std::string& manifest_path)
        throw(util::exception);
};

}}

#endif

