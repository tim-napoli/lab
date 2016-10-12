/**
 * @module      lab::parser::common
 * @brief       Give basic parsing functions.
 * @author      Timothée Napoli
 * @license     WTFPL
 * @copyright   Timothée Napoli (c) 2016
 */
#ifndef _lab_parser_common_hpp_
#define _lab_parser_common_hpp_

#include "lab/parser/parser.hpp"

namespace lab { namespace parser {

/**
 * Parse the single char `c` on `input`. `input` is consumed.
 * @return the parsed char.
 * @throw a parser::exception if `input` doesn't start with `c`.
 */
char parse_char(std::istream& input, char c)
        throw(exception);

/**
 * Parse one of the characters in `allowed` on `input`. `input` is consumed.
 * @return the parsed char.
 * @throw a parser::exception if `input` doesn't start with one char
 *        of `allowed`.
 */
char parse_one_of_chars(std::istream& input, const std::string& allowed)
        throw(exception);

/**
 * Parse one space character (' ', '\t', '\n', '\v', '\f', '\r').
 * `input` is consumed.
 * @return the parsed space char.
 * @throw a parser::exception if `input` doesn't start with a space character.
 */
char parse_space(std::istream& input)
        throw(exception);

/**
 * Parse the word `word` on `input`. `input` is consumed.
 * @return the parsed word.
 * @throw a parser::exception if `input` doesn't start with `word`.
 */
std::string parse_word(std::istream& input, const std::string& word)
        throw(exception);

}}

#endif

