#ifndef PYTHON_CATEGORY_H
#define PYTHON_CATEGORY_H

#include "python_capability.h"
#include "category.h"

#include <nlohmann/json.hpp>
#include <pybind11/embed.h>
#include <vector>
#include <string>
#include <map>

namespace py = pybind11;

class python_module;

class python_category : public category {
public:
    python_category(const std::string& name);
};

#endif