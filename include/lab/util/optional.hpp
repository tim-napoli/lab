/**
 * @module      lab::util::optional
 * @brief       An optional is a specific data type that can be either be
 *              set or not.
 * @author      Timothée Napoli
 * @license     WTFPL
 * @copyright   Timothée Napoli (c) 2016
 */
#ifndef _lab_util_optional_hpp_
#define _lab_util_optional_hpp_

#include <sstream>
#include <iostream>
#include <cassert>

namespace lab { namespace util {

/**
 * An optional is a specific data type that is used as return value for
 * functions that can either return a value or hasn't a to return.
 */
template<typename Type>
class optional {
  private:
    bool _is_set;
    Type _value;

  public:
    bool is_set()     {return _is_set;}

    const Type& get_value() const {assert(_is_set); return _value;}
    Type&       get_value() {assert(_is_set); return _value;}

    /**
     * Build an optional with a value.
     */
    static optional<Type> value(Type v) {
        optional<Type> res;
        res._is_set = true;
        res._value  = v;
        return res;
    }

    /**
     * Build an unset optional
     */
    static optional<Type> none() {
        optional<Type> res;
        res._is_set = false;
        return res;
    }
};

}}

#endif


