#include "capability.h"
#include "module.h"
#include "category.h"

#include "logger.h"

capability::capability(const std::string& name, const std::string& id)
    : m_name(name), m_id(id) {}

capability::capability(const std::string& name, const std::string& id,
    std::vector<std::shared_ptr<parameter>> params) 
    : m_name(name), m_id(id) {

    for(auto param : params) add_parameter(param);    
}


json::value capability::define() {
    json::value j;

    j[L"fullname"] = json::value::string(conversions::to_string_t(fullname()));
    j[L"name"] = json::value::string(conversions::to_string_t(m_name));
    j[L"id"]   = json::value::string(conversions::to_string_t(m_id));

    std::vector<json::value> json_params{};
    for(auto& param : m_params)
        json_params.push_back(param->define());

    j[L"childs"] = json::value::array(json_params);

    return j;
}

void capability::add_parameter(std::shared_ptr<parameter> param) {
    param->parent(this);
    // this is not multithreading safe!
    param->id(std::to_string(m_params.size()));
    m_params.push_back(param);
}

std::string capability::fullname() {
    return (m_category? m_category->fullname() : std::string("???")) + "/" + m_id;
}

std::string capability::id()     { return m_id;       }
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

json::value result::serialize() {
    json::value j;
    j[L"message"] = json::value::string(conversions::to_string_t(m_message));
    j[L"type"]    = json::value::string(conversions::to_string_t(std::to_string((int)m_type)));

    return j;
}

result::type result::result_type()    { return m_type;    }
std::string  result::result_message() { return m_message; }