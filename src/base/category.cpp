#include "module.h"
#include "category.h"

category::category(const std::string& name)
    : m_name(name) { }

category::category(const std::string& name, 
    const std::vector<std::shared_ptr<capability>> capabilities) : m_name(name) { 
        
    for(auto cap : capabilities) add_capability(cap);
}

void category::add_capability(std::shared_ptr<capability> capability) {
    capability->parent(this);
    // maybe use unique id's instead of display names?
    m_capabilities[capability->name()] = capability;
}

std::shared_ptr<capability> category::find_capability(const std::string capability) {
    try { return m_capabilities.at(capability); }
    catch(const std::out_of_range&) {
        throw std::out_of_range("capability named " + capability + " not found");
    }
}

std::string category::fullname() { 
    return (m_module? m_module->fullname() : std::string("???")) + "." + m_name; 
}

std::string category::name()   { return m_name;   }
module*     category::parent() { return m_module; }

void        category::parent(module* mod) { m_module = mod; }

result category::pass_execution(const std::string& capability, js& args) {
    return m_capabilities[capability]->execute(args);
}

js category::define() {
    js j {{ "name", m_name }};

    for(auto cap : m_capabilities)
        j["capabilities"].push_back(cap.second->define());

    return j;
}