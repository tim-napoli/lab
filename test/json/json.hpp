#include <cxxtest/TestSuite.h>
#include "lab/json/json.hpp"

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
        TS_ASSERT(my_value.get_value().get_value<int>() == 32);
    }

    void testJsonObject() {
        json my_object(std::map<std::string, json> {
            {"x", value::build<int>(32)},
            {"y", value::build<int>(16)},
        });
        TS_ASSERT(my_object.is_object());
        TS_ASSERT(my_object.get_member("x").get_value().get_value<int>()
                  == 32);
        TS_ASSERT(my_object.get_member("y").get_value().get_value<int>()
                  == 16);
        TS_ASSERT(my_object["y"].get_value().get_value<int>() == 16);

        my_object.set_member("y", value::build<int>(48));
        TS_ASSERT(my_object["y"].get_value().get_value<int>() == 48);
        my_object["y"] = value::build<int>(64);
        TS_ASSERT(my_object["y"].get_value().get_value<int>() == 64);

        TS_ASSERT_THROWS(my_object.get_member("z"), exception);
        TS_ASSERT_THROWS(my_object["z"], exception);
    }

    void testJsonArray() {
        json my_array(std::vector<json> {
            value::build<int>(32),
            value::build<int>(16),
        });
        TS_ASSERT(my_array.is_array());
        TS_ASSERT(my_array.get_element(0).get_value().get_value<int>()
                  == 32);
        TS_ASSERT(my_array.get_element(1).get_value().get_value<int>()
                  == 16);
        TS_ASSERT(my_array[1].get_value().get_value<int>() == 16);

        my_array.add_element(value::build<int>(48));
        TS_ASSERT(my_array[2].get_value().get_value<int>() == 48);

        my_array.set_element(1, value::build<int>(64));
        TS_ASSERT(my_array[1].get_value().get_value<int>() == 64);

        my_array[1] = value::build<int>(48);
        TS_ASSERT(my_array[1].get_value().get_value<int>() == 48);

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
        TS_ASSERT(my_object["position"]["x"].get_value().get_value<int>()
                  == 32);
        TS_ASSERT(my_object["position"]["y"].get_value().get_value<int>()
                  == 48);
        TS_ASSERT(my_object["numbers"][0].get_value().get_value<int>()
                  == 1);
        TS_ASSERT(my_object["numbers"][1].get_value().get_value<int>()
                  == 2);
    }
};

