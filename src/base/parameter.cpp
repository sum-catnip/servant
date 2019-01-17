#include "capability.h"
#include "parameter.h"
#include "logger.h"

parameter::parameter(const std::string& type) 
    : m_type(type) {}

parameter::parameter(const std::string& name, const std::string& type) 
    : m_name(name), m_type(type) {}

capability* parameter::parent()                { return m_capability; }
void        parameter::parent(capability* cap) { m_capability = cap; }

json::value parameter::define() {
    json::value j;

    j[L"fullname"] = json::value::string(conversions::to_string_t(fullname()));
    j[L"id"]       = json::value::string(conversions::to_string_t(m_id));
    j[L"name"]     = json::value::string(conversions::to_string_t(m_name));
    j[L"type"]     = json::value::string(conversions::to_string_t(m_type));

    return j; 
}

std::string parameter::fullname() {
    return (parent()? parent()->fullname() : std::string("???")) + "/" + m_id;
}

void parameter::id(const std::string& id) { m_id = id; }

// TEXT

text::text() : parameter(TYPE) { }
text::text(const std::string& name) : parameter(name, TYPE) { }

std::shared_ptr<parameter> text::parse(json::value& input) {
    std::shared_ptr<text> parsed = std::make_shared<text>();
    
    parsed->m_value = conversions::to_utf8string(input[L"value"].as_string());
    parsed->m_name  = m_name;
    parsed->m_id    = m_id;
    parsed->m_capability = m_capability;

    return parsed;
}

json::value text::define() { return parameter::define(); }
std::string text::value()  { return m_value; }