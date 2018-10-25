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

std::string module::fullname() { return m_name; }
std::string module::name()     { return m_name; }
std::string module::version()  { return m_version; }

void module::add_category(const std::shared_ptr<category> category) {
    m_categories[category->name()] = category;
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
    js& args) {

    return m_categories[category]->pass_execution(capability, args); 
}

js module::define() {
    js j {
        { "id",      m_id      },
        { "name",    m_name    },
        { "version", m_version },
        { "author",  m_author  }
    };

    for(auto cat : m_categories)
        j["categories"].push_back(cat.second->define());
    
    return j;
}