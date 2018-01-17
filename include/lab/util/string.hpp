#ifndef _lab_util_string_hpp_
#define _lab_util_string_hpp_

#include <string>
#include <sstream>

namespace lab { namespace util {

template <typename First>
std::string format(const std::string& fmt, First first) {
    std::ostringstream result;
    for (auto c = fmt.begin(); c != fmt.end(); c++) {
        if (*c == '{' && *(c + 1) == '}') {
            result << first;
            c++;
        } else {
            result << *c;
        }
    }
    return result.str();
}

/**
 * Build an exception using a python-style format string and a variadic
 * parameters list.
 * @code
 *      my_str = lab::util::format(
 *          "Hello {}, you're {} years old",
 *          "John", 32
 *      );
 * @endcode
 */
template <typename First, typename... Args>
std::string format(const std::string& fmt, First first, Args... args) {
    std::ostringstream result;
    for (auto c = fmt.begin(); c != fmt.end(); c++) {
        if (*c == '{' && *(c + 1) == '}') {
            result << first;
            result << format(fmt.substr(std::distance(fmt.begin(), c + 2),
                                        std::string::npos),
                             args...);
            return result.str();
        } else {
            result << *c;
        }
    }
    return result.str();
}

}}

#endif

