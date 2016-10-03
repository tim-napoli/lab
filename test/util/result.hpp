#include <cxxtest/TestSuite.h>
#include "lab/util/result.hpp"

using namespace lab::util;

#define ERRMSG  "division by zero"

static result<double> division(int a, int b) {
    if (b == 0) {
        return result<double>::error(ERRMSG);
    }
    return result<double>::success(a / b);
}

class ResultTestSuite : public CxxTest::TestSuite {
  public:
    void setUp() {

    }

    void tearDown() {

    }

    void testResult() {
        result<double> ok = division(6, 3);
        TS_ASSERT(ok.is_success());
        TS_ASSERT(ok.get_value() == 2);
        result<double> nok = division(6, 0);
        TS_ASSERT(nok.is_error());
        TS_ASSERT(nok.get_error() == ERRMSG);
    }
};

