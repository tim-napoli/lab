#include <cxxtest/TestSuite.h>
#include "lab/util/exception.hpp"

using namespace lab::util;

class ExceptionTestSuite : public CxxTest::TestSuite {
  public:
    void setUp() {

    }

    void tearDown() {

    }

    void testFormattedBuild() {
        try {
            throw exception::build_formatted("a {} c = {}", "+", 32);
        } catch(const exception& ex) {
            TS_ASSERT(ex.get_message() == "a + c = 32");
        }
    }
};

