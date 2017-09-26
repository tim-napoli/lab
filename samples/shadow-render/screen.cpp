#include "screen.hpp"
#include "constants.hpp"

using namespace lab;

screen::screen()
    : _renderer(VWIDTH, VHEIGHT)
{

}

screen::~screen() {

}

void screen::start(engine::engine_interface* intf)
    throw(util::exception)
{
    _light_position = point::build_random();
    _renderer.start(intf);
}

void screen::update(engine::engine_interface* intf)
    throw(util::exception)
{
    _light_position.move();
    _renderer.render();
}

void screen::stop(engine::engine_interface* intf)
    throw(lab::util::exception)
{
    _renderer.stop(intf);
}

void screen::notify(const event::event& evt)
    throw(util::exception)
{
    if (evt.get_source_id() == engine::window::get_id()) {
        if (evt.get_type() == engine::window::events::resized) {
            _renderer.set_windows_dimensions(
                evt.get_value("width").get<int>(),
                evt.get_value("height").get<int>()
            );
        }
    }
}
