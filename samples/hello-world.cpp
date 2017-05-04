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

    engine.plug_module(std::move(window));

    try {
        engine.start();
        engine.run();
        engine.stop();
    } catch (util::exception ex) {
        std::cout << ex.get_message() << std::endl;
    }

    return 0;
}

