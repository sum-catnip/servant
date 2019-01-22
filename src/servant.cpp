#include "servant.h"
#include "logger.h"

servant::servant(const std::string& modpath, const string_t& address)
    : m_modpath(modpath), m_listener(address), m_modules() {}

servant::~servant() {
    m_listener.close().wait();
}

void servant::start() {
    m_modules.load_modules(m_modpath);
    
    m_listener.support(methods::GET, 
        std::bind(&servant::handle_get,  this, std::placeholders::_1));

    m_listener.support(methods::POST, 
        std::bind(&servant::handle_post, this, std::placeholders::_1));

    m_listener.open().wait();
    logger::log(logger::level::INFO, "webervice started");
}

void servant::handle_get(http_request msg) {
    logger::log(logger::level::DEBUG, msg.to_string());
    std::vector<std::string> parts{};
    for(string_t part : uri::split_path(msg.relative_uri().to_string()))
        parts.push_back(conversions::to_utf8string(part));

    try {
        // in case i want to offer different endpoints in the future
        if(parts.at(0) == "modules") {
            switch(parts.size()) {
                case 1: // modules
                    msg.reply(status_codes::OK, m_modules.define());
                    break;

                case 2: // modules/module
                    msg.reply(status_codes::OK, m_modules
                        .find_module(parts[1])
                        ->define());
                    break;

                case 3: // modules/module/category
                    msg.reply(status_codes::OK, m_modules
                        .find_module(parts[1])
                        ->find_category(parts[2])
                        ->define());

                    break;

                case 4: // modules/module/category/capability
                    msg.reply(status_codes::OK, m_modules
                        .find_module(parts[1])
                        ->find_category(parts[2])
                        ->find_capability(parts[3])
                        ->define());

                    break;

                case 5: // modules/module/category/capability/0
                    msg.reply(status_codes::OK, m_modules
                        .find_module(parts[1])
                        ->find_category(parts[2])
                        ->find_capability(parts[3])
                        ->define());
                    break;

                default:
                    logger::log(logger::level::ERROR, 
                        "there was a request with an invalid URI. "
                        "either the client has a but or someine is mocking your server");

                    msg.reply(status_codes::NotFound, "provided resource path has an invaid amound of elements");
                    break;
            }
        } else msg.reply(status_codes::NotFound, "the requested endpoint was not found");
    }
    catch(const std::out_of_range& e) {
        std::string error = std::string("a requested resource was not found. details: ") + e.what();
        logger::log(logger::level::ERROR, error);
        msg.reply(status_codes::NotFound, error);
    }
}

void servant::handle_post(http_request msg) {
    logger::log(logger::level::DEBUG, msg.to_string());

    std::vector<std::string> parts{};
    for(string_t part : uri::split_path(msg.relative_uri().to_string()))
        parts.push_back(conversions::to_utf8string(part));
        
    try {
        // in case i want to offer different endpoints in the future
        if(parts.at(0) == "modules") {
            result res = m_modules.pass_execution(
                parts.at(1), 
                parts.at(2), 
                parts.at(3),
                msg.extract_json().get()
            );

            msg.reply(status_codes::OK, res.serialize());
        }
    }
    catch(const std::out_of_range& e) {
        std::string error = std::string("a requested resource was not found. details: ") + e.what();
        logger::log(logger::level::ERROR, error);
        msg.reply(status_codes::NotFound, error);
    }
    catch(const json::json_exception& e) {
        std::string error = std::string("error parsing request body. details: ") + e.what();
        logger::log(logger::level::ERROR, error);
        msg.reply(status_codes::BadRequest, error);
    }
    catch(const std::exception& e) {
        logger::log(logger::level::ERROR, 
            std::string("something bad happened while executing a capability. details: ") + e.what());
        msg.reply(status_codes::InternalError);
    }
}