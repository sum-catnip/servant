#ifndef CATEGORY_H
#define CATEGORY_H

#include "i_definable.h"
#include "capability.h"

#include <nlohmann/json.hpp>

using js = nlohmann::json;

class module;

class category : public i_definable {
public:
    category(const std::string& name);
    category(const std::string& name, 
        const std::vector<std::shared_ptr<capability>> capabilities);

    void add_capability(std::shared_ptr<capability> capability);
    std::shared_ptr<capability> find_capability(const std::string capability);

    result pass_execution(const std::string& capability, js& args);
    virtual js define() override;

    std::string name();
    std::string fullname();
    module*     parent();
    void        parent(module* mod);
protected:
    // using a shared ptr since the python interpreter cannot 
    // guarantee exclusive access to a python generated object
    std::map<std::string, std::shared_ptr<capability>> m_capabilities;
    std::string m_name;

    // obj is ded if parent is nullptr
    // so ill use a manual pointer
    module* m_module;
};

#endif