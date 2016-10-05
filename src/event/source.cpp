#include <cassert>
#include "lab/event/source.hpp"

namespace lab { namespace event {

source::source()
        : _listeners()
{

}

source::~source() {

}

void source::register_listener(listener* l) {
    assert(l != NULL);
    _listeners.push_back(l);
}

void source::unregister_listener(listener* l) {
    _listeners.remove(l);
}

void source::send_event(const event& event) throw(util::exception) {
    for (auto it = _listeners.begin(); it != _listeners.end(); it++) {
        (*it)->notify(event);
    }
}

}}

