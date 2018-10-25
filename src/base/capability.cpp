#include "capability.h"
#include "module.h"
#include "category.h"

#include "logger.h"

capability::capability(const std::string& name)
    : m_name(name) {}

capability::capability(const std::string& name, 
    std::vector<std::shared_ptr<parameter>> params) 
    : m_name(name) {

    for(auto param : params) add_parameter(param);    
}


js capability::define() {
    js j {{ "name", m_name }};

    for(auto param : m_params)
        j["parameters"].push_back(param->define());

    return j;
}

void capability::add_parameter(std::shared_ptr<parameter> param) {
    param->parent(this);
    m_params.push_back(param);
}

std::string capability::fullname() {
    return (m_category? m_category->fullname() : std::string("???") + "." + m_name);
}

std::string capability::name()   { return m_name;     }
category*   capability::parent() { return m_category; }

void        capability::parent(category* cat) { m_category = cat; }

// result class

// created a new results and loggs it if its an error
result::result(type result_type, const std::string& message)
    : m_message(message), m_type(result_type) {
    
    if(result_type == type::ERROR) {
        logger::log(logger::level::ERROR, message);
    }
}
