#ifndef SERVANT_H
#define SERVANT_H

#include "module_handler.h"

#include <string>

#include <cpprest/http_listener.h>
#include <cpprest/uri.h>
#include <cpprest/asyncrt_utils.h>

using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

// the web service implermentation
class servant {
public:
    servant(const std::string& modpath, const string_t& address);
    ~servant();

    void start();
private:
    module_handler m_modules;
    http_listener  m_listener;

    // root directory for modules
    std::string    m_modpath;

    void handle_get (http_request msg);
    void handle_post(http_request msg);
};

#endif