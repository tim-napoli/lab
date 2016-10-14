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

    void testOneOfChars() {
        std::stringstream input("bx");
        try {
            char read = parse_one_of_chars(input, "abc");
            TS_ASSERT(read == 'b');
        } catch (exception ex) {
            TS_ASSERT(false);
        }
        TS_ASSERT_THROWS(parse_one_of_chars(input, "abc"), exception);
    }

    void testParseSpace() {
        std::stringstream input(" \n\t\v\f\rx");
        try {
            char read = parse_space(input);
            TS_ASSERT(read == ' ');
            read = parse_space(input);
            TS_ASSERT(read == '\n');
            read = parse_space(input);
            TS_ASSERT(read == '\t');
            read = parse_space(input);
            TS_ASSERT(read == '\v');
            read = parse_space(input);
            TS_ASSERT(read == '\f');
            read = parse_space(input);
            TS_ASSERT(read == '\r');
        } catch (exception ex) {
            TS_ASSERT(false);
        }
        TS_ASSERT_THROWS(parse_space(input), exception);
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
        parse_many<std::string, char, char>(
            input, (function<char, char>)parse_char, ' '
        );
        try {
            std::string result = parse_word(input, "bob");
            TS_ASSERT(result == "bob");
        } catch (exception ex) {
            TS_ASSERT(false);
        }
    }

    void testParseManyStr() {
        std::stringstream input("    bob");
        std::string result = parse_many<std::string, char, char>(
            input, (function<char, char>)parse_char, ' '
        );
        TS_ASSERT(result == "    ");
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

    void testParseOptional() {
        std::stringstream input("a");
        char read = parse_optional<char, char>(
            input, 'x', (function<char, char>)parse_char, 'x'
        );
        TS_ASSERT(read == 'x');

        try {
            read = parse_char(input, 'a');
            TS_ASSERT(read == 'a');
        } catch (exception ex) {
            TS_ASSERT(false);
        }
    }
};

