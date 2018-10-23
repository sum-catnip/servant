#ifndef PYTHON_MODULE_H
#define PYTHON_MODULE_H

#include <map>
#include <string>
#include <pybind11/embed.h>
#include <nlohmann/json.hpp>

#include "python_category.h"
#include "python_capability.h"
#include "module.h"

namespace py = pybind11;
using nlohmann::json;

class python_module : public module {
public:

    python_module(
        const std::string& path, 
        const std::string& name,
        const std::string& id,
        const std::string& version,
        const std::string& author,
        const std::string& module);

private:
    // the python module
    py::module m_pymod;

    void load_module() override; 
};

#endif