#include <cxxtest/TestSuite.h>
#include "lab/util/value.hpp"

using namespace lab::util;

class ValueTestSuite : public CxxTest::TestSuite {
  public:
    void setUp() {

    }

    void tearDown() {

    }

    void testValueConversion() {
        value v = value::build(32);
        TS_ASSERT(v.get() == "32");
        TS_ASSERT(v.get<int>() == 32);
        v.set(32.325);
        TS_ASSERT(v.get() == "32.325");
        TS_ASSERT(v.get<double>() == 32.325);
    }
};

