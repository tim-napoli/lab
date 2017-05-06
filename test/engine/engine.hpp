#include <cxxtest/TestSuite.h>
#include "mocks/gl.hpp"
#include "mocks/glfw.hpp"
#include "lab/engine/engine.hpp"

using namespace lab;

class ticker : public engine::module {
  private:
    int _ticks;
    bool _started;
    bool _stopped;
    engine::engine* _engine;

  public:
    ticker(engine::engine* engine)
             : engine::module("ticker", json::json(util::value("")))
             , _ticks(0)
             , _started(false)
             , _stopped(false)
             , _engine(engine)
    {

    }

    bool is_started() const {return _started;}

    bool is_stopped() const {return _stopped;}

    void start() throw(util::exception) {
        _started = true;
    }

    void stop() throw(util::exception) {
        _stopped = true;
    }

    void update() throw(util::exception) {
        _ticks++;
        if (_ticks == 10) {
            _engine->close();
        }
    }

    void notify(const event::event& evt) throw(util::exception) {

    }

    SOURCE_MAKE_ID(ticker)
};

class EngineTestSuite : public CxxTest::TestSuite {
  public:
    void setUp() {

    }

    void tearDown() {

    }

    void testEngine() {
        engine::engine engine(60);
        std::unique_ptr<ticker> ticker_ptr(new ticker(&engine));
        const ticker* saved_ticker = ticker_ptr.get();
        engine.plug_module(std::move(ticker_ptr));

        engine.start();
        TS_ASSERT(saved_ticker->is_started());

        double start_time = glfwGetTime();
        engine.run();
        double stop_time = glfwGetTime();
        TS_ASSERT(stop_time - start_time >= 0.99 * 10 * (1.0 / 60));
        TS_ASSERT(stop_time - start_time <= 1.01 * 10 * (1.0 / 60));

        engine.stop();
        TS_ASSERT(saved_ticker->is_stopped());
    }

};

