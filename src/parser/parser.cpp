#include "lab/parser/parser.hpp"

namespace lab { namespace parser {

exception::exception(std::istream& input, const std::string& message)
        : _message(message)
{
    std::streampos initial_pos = input.tellg();
    int cursor = 0;
    int row = 1;
    int line = 1;

    input.seekg(0, input.beg);
    while (cursor < initial_pos) {
        char c = input.get();
        if (c == '\n') {
            line++;
            row = 1;
        } else {
            row++;
        }
        cursor++;
    }

    _row = row;
    _line = line;
}

exception::~exception() {

}

std::string exception::get_message() const {
    std::stringstream ss;

    ss << "at line " << _line << ", row " << _row << " : " << _message;
    return ss.str();
}

}}

