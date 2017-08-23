#include "lab/lab.hpp"
#include "screen.hpp"

using namespace lab;

int main(void) {
    engine::engine engine("shadow render", 1024, 768, false, 60);

    try {
        engine.start();
        engine.start_screen(
            std::unique_ptr<engine::screen>(new screen())
        );
        engine.run();
        engine.stop();
    } catch (util::exception ex) {
        std::cout << ex.get_message() << std::endl;
    }

    return 0;
}

