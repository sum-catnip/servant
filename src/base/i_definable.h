#ifndef I_DEFINABLE_H
#define I_DEFINABLE_H

#include <cpprest/json.h>

using namespace web;
using namespace utility;

class i_definable {
    virtual json::value define() = 0;
};

#endif