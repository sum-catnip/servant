#include "python_category.h"
#include "python_module.h"

python_category::python_category(const std::string& name, const std::string& id)
    : category(name, id) { }

python_category::python_category(const std::string& name, const std::string& id,
    const std::vector<std::shared_ptr<capability>> capabilities)
    : category(name, id, capabilities) { }