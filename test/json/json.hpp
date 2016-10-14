#include <cxxtest/TestSuite.h>
#include "lab/json/json.hpp"
#include "lab/parser/parser.hpp"

using namespace lab::json;
using namespace lab::util;

class JsonTestSuite : public CxxTest::TestSuite {
  public:
    void setUp() {

    }

    void tearDown() {

    }

    void testJsonValue() {
        json my_value(value::build<int>(32));
        TS_ASSERT(my_value.is_value());
        TS_ASSERT(my_value.get_value().get<int>() == 32);
    }

    void testJsonObject() {
        json my_object(std::map<std::string, json> {
            {"x", value::build<int>(32)},
            {"y", value::build<int>(16)},
        });
        TS_ASSERT(my_object.is_object());
        TS_ASSERT(my_object.get_member("x").get_value().get<int>()
                  == 32);
        TS_ASSERT(my_object.get_member("y").get_value().get<int>() == 16);
        TS_ASSERT(my_object["y"].get_value().get<int>() == 16);

        my_object.set_member("y", value::build<int>(48));
        TS_ASSERT(my_object["y"].get_value().get<int>() == 48);
        my_object["y"] = value::build<int>(64);
        TS_ASSERT(my_object["y"].get_value().get<int>() == 64);

        TS_ASSERT_THROWS(my_object.get_member("z"), exception);
        TS_ASSERT_THROWS(my_object["z"], exception);
    }

    void testJsonArray() {
        json my_array(std::vector<json> {
            value::build<int>(32),
            value::build<int>(16),
        });
        TS_ASSERT(my_array.is_array());
        TS_ASSERT(my_array.get_element(0).get_value().get<int>() == 32);
        TS_ASSERT(my_array.get_element(1).get_value().get<int>() == 16);
        TS_ASSERT(my_array[1].get_value().get<int>() == 16);

        my_array.add_element(value::build<int>(48));
        TS_ASSERT(my_array[2].get_value().get<int>() == 48);

        my_array.set_element(1, value::build<int>(64));
        TS_ASSERT(my_array[1].get_value().get<int>() == 64);

        my_array[1] = value::build<int>(48);
        TS_ASSERT(my_array[1].get_value().get<int>() == 48);

        TS_ASSERT_THROWS(my_array.get_element(3), exception);
        TS_ASSERT_THROWS(my_array[3], exception);
    }

    void testJsonRecursive() {
        json my_object(std::map<std::string, json> {
            {
                "position", json(std::map<std::string, json> {
                    {"x", value::build<int>(32)},
                    {"y", value::build<int>(48)},
                })
            }, {
                "numbers", json(std::vector<json> {
                    json(value::build<int>(1)),
                    json(value::build<int>(2)),
                })
            }
        });
        TS_ASSERT(my_object["position"]["x"].get_value().get<int>() == 32);
        TS_ASSERT(my_object["position"]["y"].get_value().get<int>() == 48);
        TS_ASSERT(my_object["numbers"][0].get_value().get<int>() == 1);
        TS_ASSERT(my_object["numbers"][1].get_value().get<int>() == 2);
    }

    void testParseString() {
        try {
            std::stringstream input("\"This is a valid string\"");
            std::string str = parse_string(input);
            TS_ASSERT(str == "This is a valid string");
        } catch (lab::parser::exception ex) {
            TS_ASSERT(false);
        }

        // Check control characters.
        try {
            std::stringstream input("\"\\\" \\\\ \\t \\b \\f \\n \\r\"");
            std::string str = parse_string(input);
            TS_ASSERT(str == "\" \\ \t \b \f \n \r");
        } catch (lab::parser::exception ex) {
            TS_ASSERT(false);
        }

        // Check invalid control characters.
        try {
            std::stringstream input("\"\\m\"");
            parse_string(input);
            TS_ASSERT(false);
        } catch (lab::parser::exception ex) {
        }

        // Check not a string
        try {
            std::stringstream input("This is not a valid string");
            parse_string(input);
            TS_ASSERT(false);
        } catch (lab::parser::exception ex) {
        }

        // Check unclosed string
        try {
            std::stringstream input("\"This is an \\\"unclosed string");
            parse_string(input);
            TS_ASSERT(false);
        } catch (lab::parser::exception ex) {
            // Check that we rewind the stream
            TS_ASSERT(ex.get_line() == 1);
            TS_ASSERT(ex.get_row() == 1);
        }
    }

    void testParseInt() {
        try {
            std::stringstream input("02587560");
            std::string result = parse_int(input);
            TS_ASSERT(result == "02587560");
        } catch (lab::parser::exception ex) {
            TS_ASSERT(false);
        }

        try {
            std::stringstream input("-02587560");
            std::string result = parse_int(input);
            TS_ASSERT(result == "-02587560");
        } catch (lab::parser::exception ex) {
            TS_ASSERT(false);
        }

        try {
            std::stringstream input("bob");
            parse_int(input);
            TS_ASSERT(false);
        } catch (lab::parser::exception ex) {
        }
    }

    void testParseFrac() {
        try {
            std::stringstream input(".123456");
            std::string result = parse_frac(input);
            TS_ASSERT(result == ".123456");
        } catch (lab::parser::exception ex) {
            TS_ASSERT(false);
        }

        try {
            std::stringstream input(".somethingelse");
            std::string result = parse_frac(input);
            TS_ASSERT(result == ".");
        } catch (lab::parser::exception ex) {
            TS_ASSERT(false);
        }

        try {
            std::stringstream input("somethingelse");
            parse_frac(input);
            TS_ASSERT(false);
        } catch (lab::parser::exception ex) {
        }
    }

    void testParseExp() {
        std::vector<std::string> prefixes = {
            "e", "e+", "e-", "E", "E+", "E-"
        };
        for (auto it = prefixes.begin(); it != prefixes.end(); it++) {
            std::stringstream input(*it + "325");
            try {
                std::string result = parse_exp(input);
                TS_ASSERT(result == *it + "325");
            } catch (lab::parser::exception ex) {
                TS_ASSERT(false);
            }
        }

        try {
            std::stringstream input("Bob is cool");
            parse_exp(input);
            TS_ASSERT(false);
        } catch (lab::parser::exception ex) {
        }

        for (auto it = prefixes.begin(); it != prefixes.end(); it++) {
            try {
                std::stringstream input(*it);
                parse_exp(input);
                TS_ASSERT(false);
            } catch (lab::parser::exception ex) {
            }
        }
    }

    void testParseNumeric() {
        try {
            std::stringstream input("123456");
            TS_ASSERT(parse_numeric(input) == "123456");
        } catch (lab::parser::exception ex) {
            TS_ASSERT(false);
        }

        try {
            std::stringstream input("-123456");
            TS_ASSERT(parse_numeric(input) == "-123456");
        } catch (lab::parser::exception ex) {
            TS_ASSERT(false);
        }

        try {
            std::stringstream input("-123456.32");
            TS_ASSERT(parse_numeric(input) == "-123456.32");
        } catch (lab::parser::exception ex) {
            TS_ASSERT(false);
        }

        try {
            std::stringstream input("-123456.32e-32");
            TS_ASSERT(parse_numeric(input) == "-123456.32e-32");
        } catch (lab::parser::exception ex) {
            TS_ASSERT(false);
        }

        try {
            std::stringstream input("bob");
            parse_numeric(input);
            TS_ASSERT(false);
        } catch (lab::parser::exception ex) {
        }
    }
};

