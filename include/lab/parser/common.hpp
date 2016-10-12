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
 * Parse the word `word` on `input`. `input` is consumed.
 * @return the parsed word.
 * @throw a parser::exception if `input` doesn't start with `word`.
 */
std::string parse_word(std::istream& input, const std::string& word)
        throw(exception);

}}

#endif

