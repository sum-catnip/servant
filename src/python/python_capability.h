#ifndef PYTHON_CAPABILITY_H
#define PYTHON_CAPABILITY_H

#include <pybind11/pybind11.h>
#include <string>
#include <vector>

#include "parameter.h"
#include "capability.h"

namespace py = pybind11;

class python_category;

class python_capability : public capability {
public:
    python_capability(
        category* category, 
        const std::string& name, 
        py::function capability);

    result execute(json& params) override;
private:
    // the python method
    py::function m_pycapability;
};

#endif