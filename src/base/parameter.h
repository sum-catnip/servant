#ifndef PARAMETER_H
#define PARAMETER_H

#include <string>
#include "i_definable.h"

class capability;

class parameter : public i_definable {
public:
    // can't make this unique because of a limitation in the python interpreter :(
    virtual std::shared_ptr<parameter> parse(json::value& input) = 0;
    // returns the stucture of the parameter as json (defines the values)
    virtual json::value define() override;

    parameter(const std::string& type);
    parameter(const std::string& name, const std::string& type);

    std::string fullname();
    capability* parent();
    void        parent(capability* cap);
    void        id(const std::string& id);
    std::string type();
protected:

    std::string m_type;
    std::string m_name;
    std::string m_id;
    capability* m_capability;
};

// definition:
// {
//    value: string
// }
class text : public parameter {
public:
    text();
    text(const std::string& name);

    std::shared_ptr<parameter> parse(json::value& input) override;
    json::value define() override;

    std::string value();
private:
    std::string m_value;
    const std::string TYPE = "text";
};

#endif