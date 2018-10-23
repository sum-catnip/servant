#ifndef PYTHON_CAPABILITY_H
#define PYTHON_CAPABILITY_H

#include <pybind11/pybind11.h>
#include <string>
#include <vector>

#include "parameter.h"
#include "capability.h"

namespace py = pybind11;

class python_capability : public capability {
public:
    python_capability(
        const std::string& name, 
        py::function capability);

    python_capability(
        const std::string& name, 
        py::function capability,
        const std::vector<std::shared_ptr<parameter>> params);


    result execute(js& params) override;
private:
    // the python method
    py::function m_pycapability;
};

#endif