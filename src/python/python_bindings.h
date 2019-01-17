#ifndef PYTHON_BINDINGS_H
#define PYTHON_BINDINGS_H

#include <string>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "module.h"
#include "capability.h"
#include "category.h"
#include "parameter.h"

#include "python_module.h"
#include "python_category.h"
#include "python_capability.h"

namespace py = pybind11;

PYBIND11_EMBEDDED_MODULE(servant, m) {
    py::class_<module>(m, "GenericModule")
        .def_property_readonly("name", &python_module::name)
        .def("add", &module::add_category);

    py::class_<category, std::shared_ptr<category>>(m, "GenericCategory")
        .def_property_readonly("name", &category::name)
        .def("add", &category::add_capability);

    py::class_<capability, std::shared_ptr<capability>>(m, "GenericCapability")
        .def_property_readonly("name", &capability::name)
        .def("add", &capability::add_parameter);

    py::class_<python_module, module>(m, "Module");
    
    py::class_<python_category, category, std::shared_ptr<python_category>>(m, "Category")
        .def(py::init<const std::string&, const std::string&>())
        .def(py::init<const std::string&, const std::string&, const std::vector<std::shared_ptr<capability>>>());

    py::class_<python_capability, capability, std::shared_ptr<python_capability>>(m, "Capability")
        .def(py::init<const std::string&, py::function>())
        .def(py::init<const std::string&, py::function, const std::vector<std::shared_ptr<parameter>>>())
        .def_property_readonly("function", &python_capability::pycapability);


    // PARAMETERS

    py::class_<parameter, std::shared_ptr<parameter>>(m, "BaseParameter");

    py::class_<text, parameter, std::shared_ptr<text>>(m, "Text")
        .def(py::init<const std::string&>())
        .def_property_readonly("value", &text::value);
}

#endif