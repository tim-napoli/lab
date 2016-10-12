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

