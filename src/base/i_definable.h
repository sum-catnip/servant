#ifndef I_DEFINABLE_H
#define I_DEFINABLE_H

#include <nlohmann/json.hpp>

using js = nlohmann::json;

class i_definable {
    virtual js define() = 0;
};

#endif