#include "module.h"

module::module(
    const std::string& path, 
    const std::string& name,
    const std::string& id,
    const std::string& version,
    const std::string& author,
    const std::string& module) :
    m_path(path),     
    m_name(name),     
    m_id(id),         
    m_version(version), 
    m_author(author), 
    m_modulefile(module) {
        
    // currently only inits all the members
}

std::string module::fullname() { return m_id;      }
std::string module::name()     { return m_name;    }
std::string module::version()  { return m_version; }

void module::add_category(const std::shared_ptr<category> category) {
    category->parent(this);
    m_categories[category->id()] = category;
}

std::shared_ptr<category> module::find_category(const std::string category) {
    try { return m_categories.at(category); }
    catch(const std::out_of_range&) {
        throw std::out_of_range("there was no category named " + category + " found");
    }
}

result module::pass_execution(
    const std::string& category, 
    const std::string& capability,
    json::value& args) {

    return m_categories.at(category)->pass_execution(capability, args); 
}

json::value module::define() {
    json::value j;

    j[L"id"]       = json::value::string(conversions::to_string_t(m_id));
    j[L"name"]     = json::value::string(conversions::to_string_t(m_name));
    j[L"version"]  = json::value::string(conversions::to_string_t(m_version));
    j[L"author"]   = json::value::string(conversions::to_string_t(m_author));
    j[L"fullname"] = json::value::string(conversions::to_string_t(fullname()));

    std::vector<json::value> json_categories{};
    for(auto& cat : m_categories)
        json_categories.push_back(cat.second->define());

    j[L"childs"] = json::value::array(json_categories);

    return j;
}