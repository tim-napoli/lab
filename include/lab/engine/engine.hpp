/**
 * @module      lab::engine::engine
 * @brief       The Lab Engine class.
 * @author      Timothée Napoli
 * @license     WTFPL
 * @copyright   Timothée Napoli (c) 2016
 */
#ifndef _lab_engine_engine_hpp_
#define _lab_engine_engine_hpp_

#include <memory>
#include "lab/event/listener.hpp"
#include "lab/event/source.hpp"
#include "lab/engine/module.hpp"

namespace lab { namespace engine {

/**
 * The Lab Engine (or the Great Ordonancer Of The Entire Universe And Beyond)
 * is the main class the lab library provide. It allows to do the link between
 * the system (using modules) and the game (using activities).
 * It will run at a fixed refresh rate (we say "The engine ticks every x
 * milliseconds") calling the update method of every module and of the running
 * activity.
 * The engine communicates with activities and modules using the lab event
 * system. It listens to every modules and the currently running activity.
 */
class engine : public event::listener
             , public event::source
{
  private:
    bool   _exit;
    double _refresh_rate;    /// In seconds.
    std::vector<std::unique_ptr<module>> _modules;

    void start_modules() throw(util::exception);

    void update_modules() throw(util::exception);

    void stop_modules() throw(util::exception);


  public:
    enum events : event::event::type {
        /**
         * Orders the engine to stop.
         */
        stop_engine,
    };

    /**
     * @param ticks_pers_second Times the engine ticks every seconds.
     */
    engine(int ticks_per_second);

    ~engine();

    /**
     * Plug a module to the engine. The module is set as a listener of the
     * engine and the engine is set as a listener of the module.
     * @see lab::engine::module
     */
    void plug_module(std::unique_ptr<module> module);

    /**
     * Start the engine, every of its modules and the top activity.
     */
    void start() throw(util::exception);

    /**
     * Run the engine loops until a module or the activity order to
     * stop.
     */
    void run() throw(util::exception);

    /**
     * Stop the engine.
     */
    void stop() throw(util::exception);

    void notify(const event::event& evt) throw(util::exception);
};

}}

#endif
