#include <cassert>
#include "lab/json/json.hpp"
#include "lab/parser/common.hpp"

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

/* parser ------------------------------------------------------------------ */

std::string parse_string(std::istream& input)
        throw(parser::exception)
{
    std::streampos initial_pos = input.tellg();
    parser::parse_char(input, '"');

    std::string str = "";
    char c = input.get();
    while (!input.eof() && c != '"') {
        if (c == '\\') {
            c = input.get();
            switch (c) {
              case '"' : str += '\"'; break;
              case '\\': str += '\\'; break;
              case 'b' : str += '\b'; break;
              case 'f' : str += '\f'; break;
              case 'n' : str += '\n'; break;
              case 'r' : str += '\r'; break;
              case 't' : str += '\t'; break;
              case 'u' :
                throw parser::exception(input,
                                        "this parser doesn't support the \\u "
                                        "character");
              default:
                throw parser::exception(input, "unknown control character ");
            }
        } else {
            str += c;
        }
        c = input.get();
    }

    if (c != '"') {
        // We rewind to have a better error message.
        // XXX Maybe we should add a exception(line, row, msg) constructor
        //     to don't have to do such things (with some function that will be
        //     able to get istream line and row to ease the process).
        input.seekg(initial_pos, input.beg);
        throw parser::exception(input, "unclosed string");
    }

    return str;
}

std::string parse_int(std::istream& input)
        throw(parser::exception)
{
    std::string result = "";

    /* XXX This in not really what we could call an elegant way of
     *     doing it.
     */
    result += parser::parse_optional<std::string, std::string>(
        input, "",
        (parser::function<std::string, std::string>)parser::parse_word,
        "-"
    );

    int count = 0;
    while (1) {
        try {
            result += parser::parse_one_of_chars(input, "0123456789");
            count++;
        } catch (parser::exception ex) {
            input.seekg(-1, input.cur);
            break;
        }
    }

    if (count == 0) {
        throw parser::exception(input, "cannot parse integer");
    }

    return result;
}

std::string parse_frac(std::istream& input)
        throw(parser::exception)
{
    std::string result = "";
    result += parser::parse_char(input, '.');
    // We don't need an integer behind the '.'.
    result += parser::parse_optional<std::string>(
        input, "", (parser::function<std::string>)parse_int
    );
    return result;
}

std::string parse_exp(std::istream& input)
        throw(parser::exception)
{
    std::string result = parser::parse_or<std::string>(
        input, (parser::functions<std::string>) {
            [](std::istream& input) {return parser::parse_word(input, "e-");},
            [](std::istream& input) {return parser::parse_word(input, "e+");},
            [](std::istream& input) {return parser::parse_word(input, "E+");},
            [](std::istream& input) {return parser::parse_word(input, "E-");},
            [](std::istream& input) {return parser::parse_word(input, "e");},
            [](std::istream& input) {return parser::parse_word(input, "E");},
        }
    );

    /* TODO add a range of times we need and can apply the parsing with
     *      parse_many.
     */
    size_t length = result.length();
    result += parser::parse_many<std::string, char, std::string>(
        input,
        (parser::function<char, std::string>)parser::parse_one_of_chars,
        "0123456789"
    );
    if (result.length() == length) {
        throw parser::exception(input, "an exponential notation must be "
                                       "followed by at least one digit.");
    }
    return result;
}

std::string parse_numeric(std::istream& input)
        throw(parser::exception)
{
    std::string result = parse_int(input);
    result += parser::parse_optional<std::string>(
                input, "", (parser::function<std::string>)parse_frac
    );
    result += parser::parse_optional<std::string>(
                input, "", (parser::function<std::string>)parse_exp
    );
    return result;
}

/* ------------------------------------------------------------------------- */

}}

