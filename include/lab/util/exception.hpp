/**
 * @module      lab::util::exception
 * @brief       Base exception class for the Lab Engine.
 * @author      Timothée Napoli
 * @license     WTFPL
 * @copyright   Timothée Napoli (c) 2016
 */
#ifndef _lab_util_exception_hpp_
#define _lab_util_exception_hpp_

#include <sstream>
#include <iostream>

namespace lab { namespace util {

/**
 * Base exception class.
 * This is a really simple exception that only store a human-readable
 * message.
 */
class exception : public std::exception{
  private:
    std::string _message;

  public:
    exception(const std::string& message = "") : _message(message) { }

    virtual ~exception() { }

    const std::string& get_message() const {return _message;}

    const char* what() const noexcept { return _message.c_str(); }

    /**
     * Build an exception using a python-style format string and a variadic
     * parameters list.
     * @code
     *      throw lab::util::exception::build_formatted(
     *          "Hello {}, you're {} years old",
     *          "John", 32
     *      );
     * @endcode
     */
    template<typename First>
    static exception build_formatted(const std::string& fmt, First first)
    {
        exception next_exception;
        std::ostringstream result;

        for (auto it = fmt.begin(); it != fmt.end(); it++) {
            if (*it == '{' && *(it + 1) == '}') {
                result << first;
                it++;
            } else {
                result << *it;
            }
        }

        return exception(result.str());
    }

    template<typename First, typename... Args>
    static exception build_formatted(const std::string& fmt,
                                     First first, Args... args)
    {
        exception next_exception;
        std::ostringstream result;

        for (auto it = fmt.begin(); it != fmt.end(); it++) {
            if (*it == '{' && *(it + 1) == '}') {
                result << first;
                next_exception = exception::build_formatted(
                    fmt.substr(std::distance(fmt.begin(), it + 2),
                               std::string::npos),
                    args...
                );
                result << next_exception.get_message();
                return exception(result.str());
            } else {
                result << *it;
            }
        }
        return exception(result.str());
    }

};

}}

#endif

