/**
 * @module      lab::engine::module
 * @brief       The Lab Engine module class.
 * @author      Timothée Napoli
 * @license     WTFPL
 * @copyright   Timothée Napoli (c) 2016
 */
#ifndef _lab_engine_module_hpp_
#define _lab_engine_module_hpp_

#include "lab/event/listener.hpp"
#include "lab/event/source.hpp"
#include "lab/json/json.hpp"
#include "lab/util/exception.hpp"

namespace lab { namespace engine {

/**
 * A Lab Engine's module is a component of the engine that is used to provide a
 * specific feature to the Engine. For exemple, the window, the mouse, the
 * keyboard are all handled by a engine's module.
 * A module is both a event::listener and a event::source. This allows it to
 * communicate with the Engine and with the user activities.
 */
class module : public event::source
             , public event::listener
{
  private:
    std::string _name;
    json::json  _cfg;

  public:
    module(const std::string& name, const json::json& cfg);

    virtual ~module();

    const std::string& get_name() const {return _name;}

    const json::json& get_cfg() const {return _cfg;}

    /**
     * This method is called at engine startup.
     */
    virtual void start() throw(util::exception) = 0;

    /**
     * This method is called each engine tick.
     */
    virtual void update() throw(util::exception) = 0;

    /**
     * This method is called at engine closeup.
     */
    virtual void stop() throw(util::exception) = 0;
};

}}

#endif

