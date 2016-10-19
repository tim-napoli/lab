#include "lab/engine/module.hpp"

namespace lab { namespace engine {

module::module(const std::string& name, const json::json& cfg)
        : event::source()
        , event::listener()
        , _name(name)
        , _cfg(cfg)
{

}

module::~module() {

}

}}

