#include "module.h"
#include "category.h"

category::category(const std::string& name, const std::string& id)
    : m_name(name), m_id(id) { }

category::category(const std::string& name, const std::string& id,
    const std::vector<std::shared_ptr<capability>> capabilities) : m_name(name), m_id(id) { 
        
    for(auto cap : capabilities) add_capability(cap);
}

void category::add_capability(std::shared_ptr<capability> capability) {
    capability->parent(this);
    // maybe use unique id's instead of display names?
    m_capabilities[capability->id()] = capability;
}

std::shared_ptr<capability> category::find_capability(const std::string capability) {
    try { return m_capabilities.at(capability); }
    catch(const std::out_of_range&) {
        throw std::out_of_range("capability named " + capability + " not found");
    }
}

std::string category::fullname() { 
    return (m_module? m_module->fullname() : std::string("???")) + "/" + m_id; 
}

std::string category::id()     { return m_id;     }
std::string category::name()   { return m_name;   }
module*     category::parent() { return m_module; }

void        category::parent(module* mod) { m_module = mod; }

result category::pass_execution(const std::string& capability, json::value& args) {
    return m_capabilities.at(capability)->execute(args);
}

json::value category::define() {
    json::value j;

    j[L"fullname"] = json::value::string(conversions::to_string_t(fullname()));
    j[L"name"] = json::value::string(conversions::to_string_t(m_name));
    j[L"id"]   = json::value::string(conversions::to_string_t(m_id));

    std::vector<json::value> json_capabilities{};
    for(auto& cap : m_capabilities)
        json_capabilities.push_back(cap.second->define());

    j[L"capabilities"] = json::value::array(json_capabilities);

    return j;
}