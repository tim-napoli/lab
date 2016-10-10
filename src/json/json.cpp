#include <cassert>
#include "lab/json/json.hpp"

namespace lab { namespace json {

json::json(const util::value& value)
        : _tag(json::value)
        , _value(value)
        , _object()
        , _array()
{ }

json::json(const std::map<std::string, json>& object)
        : _tag(json::object)
        , _object(object)
        , _value()
        , _array()
{ }

json::json(const std::vector<json>& array)
        : _tag(json::array)
        , _array(array)
        , _value()
        , _object()
{ }

json::~json() { }

const util::value& json::get_value() const {
    assert(_tag == json::value);
    return _value;
}

util::value& json::get_value() {
    assert(_tag == json::value);
    return _value;
}

void json::set_value(const util::value& value) {
    assert(_tag == json::value);
    _value = value;
}

const json& json::get_member(const std::string& name) const
        throw(util::exception)
{
    assert(_tag == json::object);
    auto it = _object.find(name);
    if (it == _object.end()) {
        throw util::exception::build_formatted(
            "json object has no member ", name
        );
    }

    return it->second;
}

json& json::get_member(const std::string& name)
        throw(util::exception)
{
    assert(_tag == json::object);
    auto it = _object.find(name);
    if (it == _object.end()) {
        throw util::exception::build_formatted(
            "json object has no member ", name
        );
    }

    return it->second;
}

void json::set_member(const std::string& name, const json& member) {
    assert (_tag == json::object);
    _object[name] = member;
}

const json& json::get_element(int index) const
        throw(util::exception)
{
    assert(_tag == json::array);
    try {
        return _array.at(index);
    } catch(std::out_of_range) {
        throw util::exception::build_formatted(
            "json array has no index ", index
        );
    }
}

json& json::get_element(int index)
        throw(util::exception)
{
    assert(_tag == json::array);
    try {
        return _array.at(index);
    } catch(std::out_of_range) {
        throw util::exception::build_formatted(
            "json array has no index ", index
        );
    }
}

int json::add_element(const json& member) {
    assert(_tag == json::array);
    _array.push_back(member);
    return _array.size() - 1;
}

void json::set_element(int index, const json& member)
        throw(util::exception)
{
    assert(_tag == json::array);
    try {
        _array.at(index) = member;
    } catch(std::out_of_range) {
        throw util::exception::build_formatted(
            "json array has no index ", index
        );
    }
}

const json& json::operator[](const std::string& name) const
        throw(util::exception)
{
    return get_member(name);
}

json& json::operator[](const std::string& name)
        throw(util::exception)
{
    return get_member(name);
}

const json& json::operator[](int index) const
        throw(util::exception)
{
    return get_element(index);
}

json& json::operator[](int index)
        throw(util::exception)
{
    return get_element(index);
}

}}

