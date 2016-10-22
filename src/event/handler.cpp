#include "lab/event/handler.hpp"

namespace lab { namespace event {

handler::handler()
        : _handlers()
{

}

handler::~handler() {

}

void handler::add_handler(source::id source_id, event::type event_type,
                          event_handler handler)
{
    auto& source = _handlers[source_id];
    auto& event = source[event_type];
    event.push_back(handler);
}

void handler::handle(const event& event) throw(util::exception) {
    auto source_it = _handlers.find(event.get_source_id());
    if (source_it == _handlers.end()) {
        return;
    }

    auto event_it = source_it->second.find(event.get_type());
    if (event_it == source_it->second.end()) {
        return;
    }

    for (auto it = event_it->second.begin(); it != event_it->second.end();
         it++)
    {
        (*it)(event);
    }
}

}}

