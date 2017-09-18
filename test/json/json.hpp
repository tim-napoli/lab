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

    void testParseValue() {
        try {
            std::stringstream input("\"This is some text\"");
            json result = parse_value(input);
            TS_ASSERT(result.get_value().get() == "This is some text");
        } catch(lab::parser::exception ex) {
            TS_ASSERT(false);
        }

        try {
            std::stringstream input("5687.32e10");
            json result = parse_value(input);
            TS_ASSERT(result.get_value().get<double>() == 5687.32e10);
        } catch(lab::parser::exception ex) {
            TS_ASSERT(false);
        }
    }

    void testParseArray() {
        try {
            std::stringstream input("[ ]");
            json array = parse_array(input);
        } catch (lab::parser::exception ex) {
            TS_ASSERT(false);
        }

        try {
            std::stringstream input("[ 32 , \"bob\",52.12e10   ,]");
            json array = parse_array(input);
            TS_ASSERT(array[0].get_value().get<double>() == 32);
            TS_ASSERT(array[1].get_value().get() == "bob");
            TS_ASSERT(array[2].get_value().get<double>() == 52.12e10);
        } catch (lab::parser::exception ex) {
            TS_ASSERT(false);
        }

        try {
            std::stringstream input("[ 32 , \"bob\",[52.12e10, 42 ]   ,]");
            json array = parse_array(input);
            TS_ASSERT(array[0].get_value().get<double>() == 32);
            TS_ASSERT(array[1].get_value().get() == "bob");
            TS_ASSERT(array[2].is_array());
            TS_ASSERT(array[2][0].get_value().get<double>() == 52.12e10);
            TS_ASSERT(array[2][1].get_value().get<int>() == 42);
        } catch (lab::parser::exception ex) {
            TS_ASSERT(false);
        }

        try {
            std::stringstream input("[1, 2, 3");
            parse_array(input);
            TS_ASSERT(false);
        } catch (lab::parser::exception ex) {
        }
    }

    void testParsePair() {
        try {
            std::stringstream input("\"name\" : 32");
            std::pair<std::string, json> result = parse_pair(input);
            TS_ASSERT(result.first == "name");
            TS_ASSERT(result.second.get_value().get<int>() == 32);
        } catch (lab::parser::exception ex) {
            TS_ASSERT(false);
        }

        try {
            std::stringstream input("\"name\" : xyz");
            parse_pair(input);
            TS_ASSERT(false);
        } catch (lab::parser::exception ex) {
        }

        try {
            std::stringstream input("\"name\"  32");
            parse_pair(input);
            TS_ASSERT(false);
        } catch (lab::parser::exception ex) {
        }

        try {
            std::stringstream input("name : 32");
            parse_pair(input);
            TS_ASSERT(false);
        } catch (lab::parser::exception ex) {
        }
    }

    void testParseObject() {
        try {
            std::stringstream input(
                "{\n"
                "   \"name\"    : \"John\",\n"
                "   \"age\"     : 42,\n"
                "   \"primes\"  : [2, 3, 5, 7, 11, 13],\n"
                "   \"position\": {\n"
                "       \"x\": 32.123456,\n"
                "       \"y\": 42.123456,\n"
                "   },\n"
                "}\n"
            );
            json object = parse_object(input);
            TS_ASSERT(object["name"].get_value().get() == "John");
            TS_ASSERT(object["age"].get_value().get<int>() == 42);
            TS_ASSERT(object["primes"][0].get_value().get<int>() == 2);
            TS_ASSERT(object["primes"][1].get_value().get<int>() == 3);
            TS_ASSERT(object["primes"][2].get_value().get<int>() == 5);
            TS_ASSERT(object["primes"][3].get_value().get<int>() == 7);
            TS_ASSERT(object["primes"][4].get_value().get<int>() == 11);
            TS_ASSERT(object["primes"][5].get_value().get<int>() == 13);
            TS_ASSERT(object["position"]["x"].get_value().get<double>() ==
                      32.123456);
            TS_ASSERT(object["position"]["y"].get_value().get<double>() ==
                      42.123456);
        } catch (lab::parser::exception ex) {
        }
    }

    void testArraySize() {
        json array = json(std::vector<json> {
            json(value("1")),
            json(value("2")),
            json(value("3"))
        });
        TS_ASSERT(array.size() == 3);
    }

    void testArrayIterator() {
        json array = json(std::vector<json> {
            json(value("1")),
            json(value("2")),
            json(value("3"))
        });
        auto it = array.begin();
        TS_ASSERT(it->get_value().get<int>() == 1);
        it++;
        TS_ASSERT(it->get_value().get<int>() == 2);
        it++;
        TS_ASSERT(it->get_value().get<int>() == 3);
        it++;
        TS_ASSERT(it == array.end());
    }
};

