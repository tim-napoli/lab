#include "lab/engine/module-manager.hpp"
#include "lab/util/string.hpp"

namespace lab { namespace engine {

module_manager::module_manager() : _modules() {

}

module_manager::~module_manager() {

}

void module_manager::plug_module(std::unique_ptr<module> module) {
    // XXX This is not really RAII but it's safe since an event source
    //     doesn't own the registred listener.
    //     However, we should think about making modules shared_ptr
    //     instead of unique_ptr...
    _modules.push_back(std::move(module));
}

void module_manager::start() throw(util::exception) {
    for (auto it = _modules.begin(); it != _modules.end(); it++) {
        try {
            (*it)->start();
        } catch (util::exception ex) {
            throw util::exception(util::format(
                "Engine cannot start module '", (*it)->get_name(), "': ",
                ex.get_message()
            ));
        }
    }
}

void module_manager::update() throw(util::exception) {
    for (auto it = _modules.begin(); it != _modules.end(); it++) {
        try {
            (*it)->update();
        } catch (util::exception ex) {
            throw util::exception(util::format(
                "Engine cannot update module '", (*it)->get_name(), "': ",
                ex.get_message()
            ));
        }
    }
}

void module_manager::stop() throw(util::exception) {
    for (auto it = _modules.begin(); it != _modules.end(); it++) {
        try {
            (*it)->stop();
        } catch (util::exception ex) {
            throw util::exception(util::format(
                "Engine cannot stop module '", (*it)->get_name(), "': ",
                ex.get_message()
            ));
        }
    }
}

}}

