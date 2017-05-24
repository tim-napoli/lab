/**
 * @module      lab::engine::engine_interface
 * @brief       The engine interface class.
 * @author      Timothée Napoli
 * @license     WTFPL
 * @copyright   Timothée Napoli (c) 2016
 */
#ifndef _lab_engine_engine_interface_hpp_
#define _lab_engine_engine_interface_hpp_

namespace lab { namespace engine {

class screen;

/**
 * Public interface the engine allows to other modules (screens) to use.
 */
class engine_interface {
  public:
    virtual void start_screen(std::unique_ptr<screen> scr) = 0;

    virtual void close_screen() = 0;

    virtual int get_window_width() const = 0;

    virtual int get_window_height() const = 0;
};

}}

#endif

