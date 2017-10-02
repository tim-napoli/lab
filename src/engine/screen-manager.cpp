#include "lab/engine/screen-manager.hpp"

namespace lab { namespace engine {

screen_manager::screen_manager() : _screens()
{

}

screen_manager::~screen_manager() {
    for (auto it = _screens.begin(); it != _screens.end(); it++) {
        delete it->scr;
    }
}

void screen_manager::update(engine_interface* intf) throw(util::exception) {
    if (_screens.size() > 0) {
        if (top_screen().state != started) {
            top_screen().scr->start(intf);
            register_listener(top_screen().scr);
            top_screen().state = started;
        }
        top_screen().scr->update(intf);
    }
}

void screen_manager::stop(engine_interface* intf) throw(util::exception) {
    for (auto it = _screens.begin(); it != _screens.end(); it++) {
        if (it->state != stopped) {
            it->scr->stop(intf);
        }
    }
}

void screen_manager::push_screen(screen* scr) {
    if (_screens.size() > 0) {
        unregister_listener(top_screen().scr);
        top_screen().state = paused;
    }
    _screens.push_back((screen_entry){stopped, scr});
}

void screen_manager::pop_screen(engine_interface* intf) throw(util::exception) {
    // Well, this can be dangerous if the caller of this function is
    // the running screen...
    if (_screens.size() > 0) {
        unregister_listener(top_screen().scr);
        if (top_screen().state != stopped) {
            top_screen().scr->stop(intf);
        }
        _screens.pop_back();
    }
}

void screen_manager::notify(const event::event& evt) throw(util::exception) {
    relay_event(evt);
}

}}

