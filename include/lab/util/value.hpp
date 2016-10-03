/**
 * @module      lab::util::value
 * @brief       A class to store every type of values.
 * @author      Timothée Napoli
 * @license     WTFPL
 * @copyright   Timothée Napoli (c) 2016
 */
#ifndef _lab_util_value_hpp_
#define _lab_util_value_hpp_

#include <string>
#include <sstream>

namespace lab { namespace util {

/**
 * A value is a class that stores values of any kind into string,
 * providing methods to read and write from/to any type.
 */
class value {
  private:
    std::string _value;

  public:
    /**
     * @param value The value's value as a string.
     */
    value(const std::string& value = "") : _value(value) { };

    ~value() { };

    /**
     * @return The value to the given type.
     */
    template <typename T>
    const T get_value() const {
        std::istringstream is(_value);
        T value;
        is >> value;
        return value;
    }

    const std::string& get_value() {
        return _value;
    }

    /**
     * Set the value from any type.
     */
    template <typename T>
    void set_value(const T& value) {
        std::ostringstream os;
        os << value;
        _value = os.str();
    }

    void set_value(const std::string& value) {
        _value = value;
    }

    /**
     * Build a value from any type.
     */
    template <typename T>
    static util::value build(const T& value) {
        util::value v;
        v.set_value<T>(value);
        return v;
    }
};

}}

#endif

