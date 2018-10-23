#ifndef CAPABILITY_H
#define CAPABILITY_H

#include <pybind11/embed.h>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "parameter.h"

using js = nlohmann::json;

class category;

class result {
public:
    enum class type {OK, ERROR};

    result(type result_type, const std::string& message);
private:
    std::string m_message;
    type m_type;
};

class capability {
public:
    capability(const std::string& name);
    capability(const std::string& name, 
        std::vector<std::shared_ptr<parameter>> params);

    // to be overwritten by each language specific capability
    virtual result execute(js& args) = 0;
    virtual js     define();
    
    void add_parameter(std::shared_ptr<parameter> param);

    std::string fullname();
    std::string name();
    category*   parent();
    void        parent(category* cat);
protected:
    std::string  m_name;
    category* m_category;

    std::vector<std::shared_ptr<parameter>> m_params;
};

#endif