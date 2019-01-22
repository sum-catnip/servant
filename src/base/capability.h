#ifndef CAPABILITY_H
#define CAPABILITY_H

#include <pybind11/embed.h>

#include <string>
#include <vector>

#include "i_definable.h"
#include "parameter.h"

class result {
public:
    enum class type {OK, ERROR};

    result(type result_type, const std::string& message);

    type result_type();
    std::string result_message();

    json::value serialize();
private:
    std::string m_message;
    type m_type;
};

class category;

class capability : public i_definable {
public:
    capability(const std::string& name, const std::string& id);
    capability(const std::string& name, const std::string& id,
        std::vector<std::shared_ptr<parameter>> params);

    // to be overwritten by each language specific capability
    virtual result      execute(json::value& args) = 0;
    virtual json::value define() override;
    
    void add_parameter(std::shared_ptr<parameter> param);

    std::string id();
    std::string fullname();
    std::string name();
    category*   parent();
    void        parent(category* cat);
protected:
    std::string m_name;
    std::string m_id;
    category*   m_category;

    std::vector<std::shared_ptr<parameter>> m_params;
};

#endif