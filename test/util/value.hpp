#include <cxxtest/TestSuite.h>
#include "lab/util/value.hpp"

using namespace lab::util;

struct raw_data {
    double a;
    size_t b;
};

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

    void testRawValue() {
        raw_data rd = {0.1, 32};
        value v = value::build_raw(rd);
        raw_data rdn = v.get_raw<raw_data>();
        TS_ASSERT(rdn.a == rd.a);
        TS_ASSERT(rdn.b == rd.b);
    }
};

