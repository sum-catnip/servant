#include "capability.h"
#include "parameter.h"
#include "logger.h"

capability* parameter::parent()                { return m_capability; }
void        parameter::parent(capability* cap) { m_capability = cap; }

json::value parameter::define() {
    json::value j;
    j[L"id"] = json::value::string(conversions::to_string_t(id()));
    return j; 
}


// TEXT

text::text() { }

std::shared_ptr<parameter> text::parse(json::value& input) {
    std::shared_ptr<text> parsed = std::make_shared<text>();
    parsed->m_value = conversions::to_utf8string(input[L"value"].as_string());

    return parsed;
}

json::value text::define() { return parameter::define(); }
std::string text::id()     { return "text"; }
std::string text::value()  { return m_value; }