/**
 * @module      lab::util::result
 * @brief       A result can be used by a function that can return two
 *              different values type.
 * @author      Timothée Napoli
 * @license     WTFPL
 * @copyright   Timothée Napoli (c) 2016
 */
#ifndef _lab_util_result_hpp_
#define _lab_util_result_hpp_

#include <sstream>
#include <iostream>
#include <cassert>
#include "lab/util/result.hpp"

namespace lab { namespace util {

/**
 * A result is a specific type used to describe process result that could
 * be of different (binary) form, like a process that should have a success
 * result and an error one.
 * This can be useful for error handling without the use of exceptions, as
 * we can see in Rust or Haskell languages, altough C++ has no easy way
 * to deal with them natively.
 * @code
 * result<double> division(double a, double b) {
 *    if (b == 0) {
 *        return result<double>::error("division by zero");
 *    } else {
 *      return result<double>::success(a / b);
 *    }
 * }
 *
 * result<double> ok = division(6, 3);
 * assert(ok.is_success() && ok.get_value() == 2);
 * result<double> nok = division(6, 0);
 * assert(ok.is_error());
 * @endcode
 */
template<typename Type, typename Err = std::string>
class result {
  private:
    bool _is_error;
    Type _value;
    Err  _error;

  public:
    bool is_error()   {return _is_error;}
    bool is_success() {return !_is_error;}

    const Type& get_value() const {assert(!_is_error); return _value;}
    Type&       get_value() {assert(!_is_error); return _value;}

    const Err& get_error() const {assert(_is_error); return _error;}

    /**
     * Build a success result.
     */
    static result<Type, Err> success(Type v) {
        result<Type, Err> res;
        res._is_error = false;
        res._value    = v;
        return res;
    }

    /**
     * Build an error result.
     */
    static result<Type, Err> error(Err err) {
        result<Type, Err> res;
        res._is_error = true;
        res._error    = err;
        return res;
    }
};

}}

#endif

