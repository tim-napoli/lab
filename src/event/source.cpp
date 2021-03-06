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

void source::send_event(event event) throw(util::exception) {
    event.set_source_id(typeid(*this).hash_code());
    for (auto it = _listeners.begin(); it != _listeners.end(); it++) {
        (*it)->notify(event);
    }
}

void source::relay_event(event event) throw(util::exception) {
    for (auto it = _listeners.begin(); it != _listeners.end(); it++) {
        (*it)->notify(event);
    }
}

}}

