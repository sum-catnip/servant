#ifndef PARAMETER_H
#define PARAMETER_H

#include <string>
#include "i_definable.h"

class capability;

class parameter : public i_definable {
public:
    // can't make this unique because of a limitation in the python interpreter :(
    virtual std::shared_ptr<parameter> parse(json::value& input) = 0;
    // name of the parameter type
    virtual std::string id() = 0;
    // returns the stucture of the parameter as json (defines the values)
    virtual json::value define() override;

    capability* parent();
    void        parent(capability* cap);
protected:
    // r/hmmm
    bool m_required;

    capability* m_capability;
};

// definition:
// {
//    value: string
// }
class text : public parameter {
public:
    text();

    std::shared_ptr<parameter> parse(json::value& input) override;
    json::value define() override;
    std::string id() override;

    std::string value();
private:
    std::string m_value;
};

#endif