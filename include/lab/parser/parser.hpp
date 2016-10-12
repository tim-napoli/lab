/**
 * @module      lab::parser::parser
 * @brief       Some parsing tools to build a LL(n) parser.
 * @author      Timothée Napoli
 * @license     WTFPL
 * @copyright   Timothée Napoli (c) 2016
 */
#ifndef _lab_parser_parser_hpp_
#define _lab_parser_parser_hpp_

#include <exception>
#include <functional>
#include <list>
#include <sstream>
#include <string>
#include <vector>
#include "lab/util/result.hpp"

namespace lab { namespace parser {

/**
 * Parser exception class.
 * Should be (or must be) used by every parser function.
 */
class exception {
  private:
    int _line;
    int _row;
    std::string _message;

  public:
    exception(std::istream& stream, const std::string& message);

    ~exception();

    std::string get_message() const;

    int get_line() const {return _line;}

    int get_row() const {return _row;}
};

/**
 * Parser function definition.
 */
template <typename T, typename... Args>
using function = std::function<T(std::istream&, Args...)>;

/**
 * Array of parser functions.
 */
template <typename T, typename... Args>
using functions = std::vector<function<T, Args...>>;

/**
 * This function will try to parse on input using parser functions given in
 * the `parsers` array.
 * If no parser function succeed, a parser::exception is thrown.
 * If `Args...` is specified, theses variadic arguments are passed to
 * every parser function.
 */
template <typename T, typename... Args>
T parse_or(std::istream& input,
           const std::vector<function<T, Args...>>& parsers,
           Args... args)
        throw(exception)
{
    std::streampos initial_pos = input.tellg();
    for (auto parser = parsers.begin(); parser != parsers.end(); parser++) {
        try {
            return (*parser)(input, args...);
        } catch (exception ex) {
            input.seekg(initial_pos, input.beg);
        }
    }
    throw exception(input, "");
}

/**
 * Applies zero or more times the parser function `parser` on input.
 * No extra characters are consumed from `input`.
 * If `Args` is specified, they are extra arguments passed to `parser`.
 * @return a list of parsed results.
 */
template <typename T, typename... Args>
std::list<T> parse_many(std::istream& input, function<T, Args...> parser,
                        Args... args)
{
    std::list<T> result_list;
    while (1) {
        std::streampos initial_pos = input.tellg();
        try {
            T result = parser(input, args...);
            result_list.push_back(result);
        } catch (exception ex) {
            input.seekg(initial_pos, input.beg);
            break;
        }
    }
    return result_list;
}

/**
 * Applies zero or more times the parser function `parser` on input.
 * No extra characters are consumed from `input`.
 * If `Args` is specified, they are extra arguments passed to `parser`.
 * @return the number of times the parser has been successfuly applied.
 */
template <typename T, typename... Args>
int skip_many(std::istream& input, function<T, Args...> parser,
              Args... args)
{
    int count = 0;
    while (1) {
        std::streampos initial_pos = input.tellg();
        try {
            parser(input, args...);
        } catch (exception ex) {
            input.seekg(initial_pos, input.beg);
            break;
        }
        count++;
    }
    return count;
}

/**
 * Try to parse on `input` with the `parser` function. If `parser` fail,
 * returns `deflt`, otherwise, returns the parser result.
 * This function doesn't consume characters if `parser` failed.
 */
template <typename T, typename... Args>
T parse_optional(std::istream& input, T deflt, function<T, Args...> parser,
                 Args... args)
{
    std::streampos initial_pos = input.tellg();
    try {
        return parser(input, args...);
    } catch (exception ex) {
        input.seekg(initial_pos, input.beg);
        return deflt;
    }
}

}}

#endif

