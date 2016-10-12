#include <cxxtest/TestSuite.h>
#include "lab/parser/parser.hpp"
#include "lab/parser/common.hpp"

using namespace lab::parser;
using namespace lab::util;

class ParserTestSuite : public CxxTest::TestSuite {
  public:
    void setUp() {

    }

    void tearDown() {

    }

    void testException() {
        std::stringstream input("a\nb\nc");
        exception ex(input, "");

        TS_ASSERT(ex.get_line() == 1);
        TS_ASSERT(ex.get_row() == 1);

        input.get();
        ex = exception(input, "");
        TS_ASSERT(ex.get_line() == 1);
        TS_ASSERT(ex.get_row() == 2);

        input.get();
        ex = exception(input, "");
        TS_ASSERT(ex.get_line() == 2);
        TS_ASSERT(ex.get_row() == 1);

        input.get();
        ex = exception(input, "");
        TS_ASSERT(ex.get_line() == 2);
        TS_ASSERT(ex.get_row() == 2);
    }

    void testParserChar() {
        std::stringstream input("aa");
        try {
            char c = parse_char(input, 'a');
            TS_ASSERT(c == 'a');
        } catch (exception ex) {
            TS_ASSERT(false);
        }

        TS_ASSERT_THROWS(parse_char(input, 'b'), exception)
    }

    void testParserWord() {
        std::stringstream input("blectrejojoxyz");
        try {
            std::string word = parse_word(input, "blectre");
            TS_ASSERT(word == "blectre");
            word = parse_word(input, "jojo");
            TS_ASSERT(word == "jojo");
        } catch (exception ex) {
            TS_ASSERT(false);
        }

        TS_ASSERT_THROWS(parse_word(input, "zyx"), exception);
    }

    void testParserOr() {
        std::stringstream input("abcdefg");
        try {
            char result = parse_or<char>(input, functions<char> {
                [](std::istream& input) {
                    return parse_char(input, 'x');
                },
                [](std::istream& input) {
                    return parse_char(input, 'a');
                }
            });
            TS_ASSERT(result == 'a');
        } catch (exception ex) {
            TS_ASSERT(false);
        }

        // Now we ensure we don't consume input in failing case.
        try {
            char result = parse_or<char>(input, functions<char> {
                [](std::istream& input) {
                    return parse_char(input, 'x');
                },
            });
            TS_ASSERT(false);
        } catch (exception ex) {
        }

        try {
            char result = parse_char(input, 'b');
            TS_ASSERT(result == 'b');
        } catch (exception ex) {
            TS_ASSERT(false);
        }
    }

    void testParseMany() {
        std::stringstream input("    bob");
        parse_many<char, char>(input, (function<char, char>)parse_char, ' ');
        try {
            std::string result = parse_word(input, "bob");
            TS_ASSERT(result == "bob");
        } catch (exception ex) {
            TS_ASSERT(false);
        }
    }

    void testSkipMany() {
        std::stringstream input("    bob");
        skip_many<char, char>(input, (function<char, char>)parse_char, ' ');
        try {
            std::string result = parse_word(input, "bob");
            TS_ASSERT(result == "bob");
        } catch (exception ex) {
            TS_ASSERT(false);
        }
    }
};

