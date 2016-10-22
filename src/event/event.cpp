#include "lab/event/event.hpp"

namespace lab { namespace event {

event::event(type type)
        : _type(type)
        , _values()
{

}

event::~event() {

}

void event::set_value(const std::string& name, const util::value& value) {
    if (_values.find(name) != _values.end()) {
        _values[name] = value;
    } else {
        _values.emplace(name, value);
    }
}

const util::value& event::get_value(const std::string& name) const
        throw(util::exception)
{
    auto it = _values.find(name);
    if (it == _values.end()) {
        throw util::exception("coudln't find event value \"" + name + "\"");
    }
    return it->second;
}

}}

