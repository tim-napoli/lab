#include <cctype>
#include "lab/parser/common.hpp"

namespace lab { namespace parser {

char parse_char(std::istream& input, char c)
        throw(exception)
{
    char read = input.get();
    if (read != c) {
        throw exception(input, "cannot read char");
    }
    return c;
}

char parse_one_of_chars(std::istream& input, const std::string& allowed)
        throw(exception)
{
    char read = input.get();
    if (allowed.find(read) == std::string::npos) {
        throw exception(input, "cannot parse one of char");
    }
    return read;
}

char parse_space(std::istream& input)
        throw(exception)
{
    char read = input.get();
    if (!isspace(read)) {
        throw exception(input, "cannot parse space");
    }
    return read;
}

std::string parse_word(std::istream& input, const std::string& word)
        throw(exception)
{
    for (auto it = word.begin(); it != word.end(); it++) {
        try {
            parse_char(input, *it);
        } catch (exception ex) {
            throw exception(input, "cannot read word");
        }
    }
    return word;
}

}}

