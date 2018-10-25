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
    for(string_t part : uri::split_path(msg.relative_uri().to_string())) {
        parts.push_back(conversions::to_utf8string(part));
    }

    try {

        if(parts.at(0) == "modules") {
            switch(parts.size()) {
                case 1: // modules
                    msg.reply(status_codes::OK, m_modules
                        .define()
                        .dump());
                    break;

                case 3: // modules/module/version
                    msg.reply(status_codes::OK, m_modules
                        .find_module(parts[1], parts[2])
                        ->define()
                        .dump());
                    break;

                case 4: // modules/module/version/category
                    msg.reply(status_codes::OK, m_modules
                        .find_module(parts[1], parts[2])
                        ->find_category(parts[3])
                        ->define()
                        .dump());

                    break;

                case 5: // modules/module/version/category/capability
                    msg.reply(status_codes::OK, m_modules
                        .find_module(parts[1], parts[2])
                        ->find_category(parts[3])
                        ->find_capability(parts[4])
                        ->define()
                        .dump());

                    break;

                case 6: // modules/module/version/category/capability/0
                    msg.reply(status_codes::OK, m_modules
                        .find_module(parts[1], parts[2])
                        ->find_category(parts[3])
                        ->find_capability(parts[4])
                        ->define()
                        .dump());
                    break;

                default:
                    logger::log(logger::level::ERROR, 
                        "there was a request with an invalid URI. "
                        "either the client has a but or someine is mocking your server");

                    msg.reply(status_codes::NotFound);
                    break;
            }
        }
    }
    catch(const std::out_of_range& e) {
        std::string error = std::string("a requested resource was not found. details: ") + e.what();
        logger::log(logger::level::ERROR, error);
        msg.reply(status_codes::NotFound, error);
    }
}

void servant::handle_post(http_request msg) {
    logger::log(logger::level::DEBUG, msg.to_string());
}