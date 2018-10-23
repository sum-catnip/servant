#ifndef PYTHON_BINDINGS_H
#define PYTHON_BINDINGS_H

#include <string>
#include <pybind11/pybind11.h>

#include "module.h"
#include "capability.h"
#include "category.h"

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
        .def_property_readonly("name", &capability::name);

    py::class_<python_module, module>(m, "Module");
    
    py::class_<python_category, category, std::shared_ptr<python_category>>(m, "Category")
        .def(py::init<python_module*, const std::string&>());

    py::class_<python_capability, capability, std::shared_ptr<python_capability>>(m, "Capability")
        .def(py::init<python_category*, const std::string&, py::function>());

    // TODO: change the constructors as noted in testmod1
    py::class_<parameter, std::shared_ptr<parameter>>(m, "Parameter")
        .def(py::init<python_category*, const std::string&, py::function>());
}

#endif