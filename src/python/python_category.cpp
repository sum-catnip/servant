#include "python_category.h"
#include "python_module.h"

python_category::python_category(const std::string& name)
    : category(name) { }

python_category::python_category(const std::string& name, 
    const std::vector<std::shared_ptr<capability>> capabilities)
    : category(name, capabilities) { }