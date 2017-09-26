#include <cstring>
#include <libgen.h>
#include "lab/json/json.hpp"
#include "lab/data/manifest.hpp"
#include "lab/data/loaders.hpp"

namespace lab { namespace data {

manifest::manifest() : _textures()
                     , _images()
                     , _animations()
{

}

manifest::~manifest() {

}

gfx::texture manifest::get_texture(const std::string& texture) const
        throw(util::exception)
{
    auto it = _textures.find(texture);
    if (it == _textures.end()) {
        throw util::exception::build_formatted(
            "cannot find texture {}", texture
        );
    }
    return it->second;
}

gfx::image manifest::get_image(const std::string& image) const
        throw(util::exception)
{
    auto it = _images.find(image);
    if (it == _images.end()) {
        throw util::exception::build_formatted(
            "cannot find image {}", image
        );
    }
    return it->second;
}

const gfx::image* manifest::get_image_ptr(const std::string& image) const
        throw(util::exception)
{
    auto it = _images.find(image);
    if (it == _images.end()) {
        throw util::exception::build_formatted(
            "cannot find image {}", image
        );
    }
    return &it->second;
}

gfx::animation manifest::get_animation(const std::string& animation) const
        throw(util::exception)
{
    auto it = _animations.find(animation);
    if (it == _animations.end()) {
        throw util::exception::build_formatted(
            "cannot find animation {}", animation
        );
    }
    return it->second;
}

void manifest::destroy() {
    for (auto it = _animations.begin(); it != _animations.end(); it++) {
        it->second.destroy();
    }
    for (auto it = _images.begin(); it != _images.end(); it++) {
        it->second.destroy();
    }
    for (auto it = _textures.begin(); it != _textures.end(); it++) {
        it->second.destroy();
    }
}

manifest manifest::load(const std::string& manifest_path)
        throw(util::exception)
{
    char manifest_path_c_str_dir[1024];
    strcpy(manifest_path_c_str_dir, manifest_path.c_str());
    char* directory_path = dirname(manifest_path_c_str_dir);

    json::json content = json::json::parse(manifest_path);
    manifest manifest;

    for (auto it = content["textures"].begin();
         it != content["textures"].end();
         it++)
    {
        std::string texture_name = it->get_value().get();
        std::string texture_path = std::string(directory_path)
                                 + "/textures/" + texture_name;
        try {
            manifest._textures[texture_name] = load_texture(texture_path);
        } catch(util::exception ex) {
            throw util::exception::build_formatted(
                "error loading texture {}: {}", texture_name, ex.get_message()
            );
        }
    }

    for (auto it = content["images"].begin();
         it != content["images"].end();
         it++)
    {
        std::string image_name = it->get_value().get();
        std::string image_path = std::string(directory_path)
                               + "/images/" + image_name;
        try {
            manifest._images[image_name] = load_image(
                manifest._textures, image_path
            );
        } catch(util::exception ex) {
            throw util::exception::build_formatted(
                "error loading image {}: {}", image_name, ex.get_message()
            );
        }
    }

    for (auto it = content["animations"].begin();
         it != content["animations"].end();
         it++)
    {
        std::string anim_name = it->get_value().get();
        std::string anim_path = std::string(directory_path)
                               + "/animations/" + anim_name;
        try {
            manifest._animations[anim_name] = load_animation(
                manifest._images, anim_path
            );
        } catch(util::exception ex) {
            throw util::exception::build_formatted(
                "error loading animation {}: {}", anim_name, ex.get_message()
            );
        }
    }

    return manifest;
}


}}

