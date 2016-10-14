/**
 * @module      lab::json::json
 * @brief       A json class to use json file format.
 * @author      Timothée Napoli
 * @license     WTFPL
 * @copyright   Timothée Napoli (c) 2016
 */
#ifndef _lab_json_json_hpp_
#define _lab_json_json_hpp_

#include <string>
#include <map>
#include <vector>

#include "lab/util/value.hpp"
#include "lab/util/exception.hpp"
#include "lab/parser/parser.hpp"

namespace lab { namespace json {

/**
 * A Json object.
 * This class is used to modelize either json scalar values, json objects and
 * json arrays.
 */
class json {
  private:
    enum {value, object, array} _tag;
    util::value                 _value;
    std::map<std::string, json> _object;
    std::vector<json>           _array;

  public:
    /**
     * Build a json value.
     */
    json(const util::value& value = util::value(""));

    /**
     * Build a json object.
     */
    json(const std::map<std::string, json>& object);

    /**
     * Build a json array.
     */
    json(const std::vector<json>& array);

    ~json();

    /**
     * @return true if this json object is a value.
     */
    bool is_value()  const {return _tag == json::value;}

    /**
     * @return true if this json object is an object.
     */
    bool is_object() const {return _tag == json::object;}

    /**
     * @return true if this json object is an array.
     */
    bool is_array()  const {return _tag == json::array;}

    /**
     * @return this json's value.
     * @pre this->is_value()
     */
    const util::value& get_value() const;
    util::value& get_value();

    /**
     * Set this json value.
     * @pre this->is_value()
     */
    void set_value(const util::value& value);

    /**
     * @return the member `name` of this json.
     * @throw if member doesn't exist.
     * @pre this->is_object()
     */
    const json& get_member(const std::string& name) const
        throw(util::exception);
    json& get_member(const std::string& name)
        throw(util::exception);

    /**
     * Set the member `name` of this json object.
     * @pre this->is_object()
     */
    void set_member(const std::string& name, const json& member);

    /**
     * @return the `index`th element of this json array.
     * @throw if index is out of bounds.
     * @pre this->is_array()
     */
    const json& get_element(int index) const
        throw(util::exception);
    json& get_element(int index)
        throw(util::exception);

    /**
     * Add an element to this json array.
     * @return the index of the inserted element.
     * @pre this->is_array()
     */
    int add_element(const json& member);

    /**
     * Set the `index`th element of this json array.
     * @throw if index is out of bounds.
     * @pre this->is_array()
     */
    void set_element(int index, const json& member)
        throw(util::exception);

    /**
     * @see get_member
     */
    const json& operator[](const std::string& name) const
        throw(util::exception);
    json& operator[](const std::string& name)
        throw(util::exception);

    /**
     * @see get_element
     */
    const json& operator[](int index) const
        throw(util::exception);
    json& operator[](int index)
        throw(util::exception);
};

std::string parse_string(std::istream& input)
        throw(parser::exception);

std::string parse_int(std::istream& input)
        throw(parser::exception);

std::string parse_frac(std::istream& input)
        throw(parser::exception);

std::string parse_exp(std::istream& input)
        throw(parser::exception);

std::string parse_numeric(std::istream& input)
        throw(parser::exception);

json parse_value(std::istream& input)
        throw(parser::exception);

}}

#endif

