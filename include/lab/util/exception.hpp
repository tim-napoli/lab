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
};

}}

#endif

