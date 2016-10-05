#include <cxxtest/TestSuite.h>
#include "lab/event/event.hpp"

using namespace lab::event;
using namespace lab::util;

class EventTestSuite : public CxxTest::TestSuite {
  public:
    void setUp() {

    }

    void tearDown() {

    }

    void testEvent() {
        event evt(0);
        TS_ASSERT(evt.get_type() == 0);

        evt.set_type(24);
        TS_ASSERT(evt.get_type() == 24);

        evt.set_value("first-value", value::build<int>(32));
        TS_ASSERT(evt.get_value("first-value").get_value<int>() == 32);

        evt.set_value("second-value", value::build<double>(3.14));
        TS_ASSERT(evt.get_value("second-value").get_value<double>() == 3.14);

        evt.set_value("first-value", value::build<double>(3.14));
        TS_ASSERT(evt.get_value("first-value").get_value<double>() == 3.14);

        TS_ASSERT_THROWS(evt.get_value("i-dont-exist"), exception);
    }
};

