#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "lab/gfx/renderer.hpp"
#include "lab/engine/window.hpp"

namespace lab { namespace gfx {

renderer::renderer(const json::json& cfg)
    : module("renderer", cfg)
{

}

renderer::~renderer() {

}

void renderer::resize(int win_width, int win_height) {
    glm::vec2 virt_dims(get_cfg()["virtual_width"].get_value().get<int>(),
                        get_cfg()["virtual_height"].get_value().get<int>());
    glm::vec2 real_dims(win_width, win_height);

    glm::vec2 ratios = real_dims / virt_dims;
    float ratio = (ratios.x * virt_dims.y > real_dims.y)
                ? ratios.y : ratios.x;

    glm::vec2 transposed_dims = virt_dims * ratio;
    glm::vec2 top_left = (real_dims - transposed_dims) / 2.0f;

    glViewport(top_left.x, top_left.y, transposed_dims.x,
               transposed_dims.y);

    _projection_matrix = glm::ortho(0.0f, transposed_dims.x,
                                    0.0f, transposed_dims.y);

    _real_to_virt_matrix =
        glm::scale(glm::mat4(1.0), glm::vec3(ratio, ratio, 0.0))
      * glm::translate(glm::mat4(1.0), glm::vec3(-top_left.x, -top_left.y, 0.0));

    send_event(event::event(renderer::events::resized, {
        {"real_to_virt", util::value::build_raw(_real_to_virt_matrix)}
    }));
}

void renderer::start() throw(util::exception) {
    // Just check the configuration is OK.
    get_cfg()["virtual_width"];
    get_cfg()["virtual_height"];
}

void renderer::update() throw(util::exception) {

}

void renderer::stop() throw(util::exception) {

}

void renderer::notify(const event::event& evt)
    throw(util::exception)
{
    if (evt.get_source_id() == engine::window::get_id()) {
        if (evt.get_type() == engine::window::events::resized) {
            resize(evt.get_value("width").get<int>(),
                   evt.get_value("height").get<int>());
        }
    }
}

}}

