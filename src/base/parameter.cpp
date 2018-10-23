#include "capability.h"
#include "parameter.h"
#include "logger.h"

capability* parameter::parent()                { return m_capability; }
void        parameter::parent(capability* cap) { m_capability = cap; }

js parameter::define() {
    return {{"id", id()}};
}

std::shared_ptr<parameter> text::parse(const js& input) {
    std::shared_ptr<text> parsed = std::make_shared<text>();

    try { parsed->m_value = input["value"].get<std::string>(); }
    catch (const js::parse_error&) {
        // TODO: throw other exception if required
        return nullptr;
    }

    return nullptr;
}

js text::define() {
    return parameter::define();
}

std::string text::id() { return "text"; }