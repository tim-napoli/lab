#include "lab/data/loaders.hpp"
#include "lab/json/json.hpp"
#include "lab/util/string.hpp"

namespace lab { namespace data {

gfx::texture load_texture(const std::string& texture_path)
       throw(util::exception)
{
    return gfx::texture::load(texture_path);
}

gfx::image load_image(const std::map<std::string, gfx::texture>& textures,
                      const std::string& image_path)
       throw(util::exception)
{
    json::json image_content = json::json::parse(image_path);
    std::vector<gfx::texture> image_textures;
    for (auto it = image_content["textures"].begin();
         it != image_content["textures"].end();
         it++)
    {
        image_textures.push_back(textures.at(it->get_value().get()));
    }
    glm::vec2 hot_point = glm::vec2(
        image_content["hot_point"]["x"].get_value().get<float>(),
        image_content["hot_point"]["y"].get_value().get<float>()
    );
    return gfx::image(image_textures, hot_point);
}

static gfx::animation_frame
load_frame(const std::map<std::string, gfx::image>& images,
           const json::json& json_node)
        throw(util::exception)
{
    gfx::image image;
    auto it = images.find(json_node["image"].get_value().get());
    if (it == images.end()) {
        throw util::exception(util::format(
            "cannot find image {}", json_node["image"].get_value().get()
        ));
    }
    image = it->second;

    return gfx::animation_frame(
        image,
        json_node["tick_duration"].get_value().get<int>()
    );
}

gfx::animation load_animation(const std::map<std::string, gfx::image>& images,
                              const std::string animation_path)
        throw(util::exception)
{
    json::json content = json::json::parse(animation_path);
    std::vector<gfx::animation_frame> frames;
    for (auto it = content["frames"].begin();
         it != content["frames"].end();
         it++)
    {
        frames.push_back(load_frame(images, *it));
    }
    return gfx::animation(frames);
}

}}

