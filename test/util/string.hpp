#include <cxxtest/TestSuite.h>
#include "lab/util/string.hpp"

using namespace lab::util;

class StringTestSuite : public CxxTest::TestSuite {
  public:
    void setUp() {

    }

    void tearDown() {

    }

    void testFormat() {
        std::string str = format("{} + {} = {}", 1, 2, 3);
        TS_ASSERT_EQUALS(str, "1 + 2 = 3");
    }
};

