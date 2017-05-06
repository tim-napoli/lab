#include "lab/lab.hpp"

using namespace lab;

int main(void) {
    engine::engine engine(60);
    std::unique_ptr<engine::window> window(
        new engine::window("Hello world", json::json({
            {"width", util::value::build<int>(400)},
            {"height", util::value::build<int>(300)},
            {"fullscreen", util::value::build<bool>(false)}
        }))
    );
    std::unique_ptr<gfx::renderer> renderer(
        new gfx::renderer(json::json({
            {"virtual_width", util::value::build<int>(800)},
            {"virtual_height", util::value::build<int>(450)},
        }))
    );

    window->register_listener(&engine);
    window->register_listener(renderer.get());

    engine.plug_module(std::move(window));
    engine.plug_module(std::move(renderer));

    try {
        engine.start();
        engine.run();
        engine.stop();
    } catch (util::exception ex) {
        std::cout << ex.get_message() << std::endl;
    }

    return 0;
}

