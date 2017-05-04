/**
 * @module      lab::engine::module_manager
 * @brief       Engine part that manages modules.
 * @author      Timothée Napoli
 * @license     WTFPL
 * @copyright   Timothée Napoli (c) 2016
 */
#ifndef _lab_engine_module_manager_hpp_
#define _lab_engine_module_manager_hpp_

#include <memory>
#include "lab/event/listener.hpp"
#include "lab/event/source.hpp"
#include "lab/engine/module.hpp"

namespace lab { namespace engine {

/**
 * The module manager has the responsibility to start, stop and update modules.
 * It also handle modules communications.
 */
class module_manager : public event::listener
                     , public event::source
{
  private:
    std::vector<std::unique_ptr<module>> _modules;

  public:
    module_manager();

    ~module_manager();

    /**
     * Plug a module to the engine. The module is set as a listener of the
     * engine and the engine is set as a listener of the module.
     * @see lab::engine::module
     */
    void plug_module(std::unique_ptr<module> module);

    void start() throw(util::exception);

    void update() throw(util::exception);

    void stop() throw(util::exception);

    void notify(const event::event& evt) throw(util::exception);

    SOURCE_MAKE_ID(module_manager);
};

}}

#endif

