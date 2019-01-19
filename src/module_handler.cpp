#include <iostream>
#include <filesystem>

#include "logger.h"
#include "module_handler.h"

module_handler::module_handler() {}

void module_handler::load_modules(const std::string& modules_path) {
    logger::log(logger::level::DEBUG, "loading stuff");
    fs::path modpath = fs::path(modules_path);
    fs::directory_iterator modules_iter;

    try { modules_iter = fs::directory_iterator(modpath); }
    catch(fs::filesystem_error& ex) {
        // 2 is file not found btw
        // there pops is a const somwhere didn't find it tho
        if(ex.code().value() == 2) { logger::log(logger::level::ERROR,
            "the module directory [" + modules_path + "] "
            "does not exist");
        }

        else { logger::log(logger::level::ERROR,
            "failed to access module directory at [" + modules_path + "] "
            "error: " + ex.what());
        }

        // rethrow cuz critical error
        throw;
    }

    for(fs::directory_entry item : modules_iter) {
        if(item.is_directory()) {
            try {
                // if module contains config file
                // division operator concats path's btw
                fs::path config_path = item.path() / CONFIG_FILENAME;
                fs::directory_entry config_file = fs::directory_entry(config_path);
                if(config_file.is_regular_file()) {
                    std::ifstream config_s(config_path.string());
                    load_module(item.path(), json::value::parse(config_s));
                }
            }
            catch(fs::filesystem_error& ex) {
                // 2 is file not found btw
                // there pops is a const somwhere didn't find it tho
                if(ex.code().value() == 2) { logger::log(logger::level::ERROR,
                    "failed to load module at [" + item.path().string() + "] "
                    "because there is no config file");
                }

                else { logger::log(logger::level::ERROR,
                    "failed to load module at [" + item.path().string() + "] "
                    "error: " + ex.what());
                }
            }
            catch(json::json_exception& ex) {
                logger::log(logger::level::ERROR,
                    "failed to load module at [" + item.path().string() + "] "
                    "because the config was not a valid json string. \n"
                    "ex: " + ex.what());
            }
        }

        // if there was a non directory entry in the mod dir
        else { logger::log(logger::level::ERROR, 
            "there was a non module file in the modules folder: [" + item.path().string() + "]");
        }
    }
}

result module_handler::pass_execution(
    const std::string& module_id,
    const std::string& category,
    const std::string& capability,
    json::value& args) {
        
    try { return find_module(module_id)->pass_execution(category, capability, args); }
    catch(std::out_of_range& ex) {
        std::string capability_full = 
            module_id + "/" + category + "/" + capability;

        std::string error = 
            "there was an attempt to execute capability [" + capability_full + "] "
            "but there was a broken link in the execution chain. "
            "please report this error to the servant authors if this keeps happening. "
            "maybe the module author forgot to change the module version? :P "
            "exception: " + ex.what();

        return result(result::type::ERROR, error);
    }
}

json::value module_handler::define() {
    json::value j;
    std::vector<json::value> json_modules{};

    j[L"id"]       = json::value::string(conversions::to_string_t(""));
    j[L"fullname"] = json::value::string(conversions::to_string_t(""));
    j[L"name"]     = json::value::string(conversions::to_string_t(""));

    for(auto& mod : m_modules)
        json_modules.push_back(mod.second->define());

    j[L"childs"] = json::value::array(json_modules);

    return j;
}

void module_handler::load_module(const fs::path& modpath, json::value& config_j) {
    std::string id; 
    std::string lang;
    std::string name;
    std::string author;
    std::string version;
    std::string modfile;

    try {
        // on windows string_t is a typedef to std::wstring but std::string on linux...
        id      = conversions::to_utf8string(config_j[L"id"].as_string());
        lang    = conversions::to_utf8string(config_j[L"lang"].as_string());
        name    = conversions::to_utf8string(config_j[L"name"].as_string());
        author  = conversions::to_utf8string(config_j[L"author"].as_string());
        version = conversions::to_utf8string(config_j[L"version"].as_string());
        modfile = conversions::to_utf8string(config_j[L"modfile"].as_string());
    }
    catch(json::json_exception e) {
        logger::log(logger::level::ERROR, 
            "module at [" + modpath.string() + "] has an invalid config. details: " + e.what());

        return;
    }

    // check if modfile exists and transform to absolute path
    try { modfile = (modpath / modfile).string(); }
    catch(fs::filesystem_error& ex) {
        // 2 is file not found btw
        // there pops is a const somwhere didn't find it tho
        if(ex.code().value() == 2) { logger::log(logger::level::ERROR,
            "the config-specified modfile [" + modfile + "] in module at "
            "[" + modpath.string() + "] did't exist. please check you config");
        }

        else { logger::log(logger::level::ERROR,
            "the config-specified modfile [" + modfile + "] in module at "
            "[" + modpath.string() + "] failed to load. error: " + 
            ex.what());
        }
    }

    // need to string2enum cuz no string switches in cpp :(
    switch(parse_lang(lang)) {
        case language::python:
            // append new python module and upcast to module
            m_modules[id] = std::make_unique<python_module>(
                modpath.string(), name, id, version, author, modfile);

            //m_modules.push_back();

            break;
        
        case language::error:
            logger::log(logger::level::ERROR, 
                "module [" + name + "] at [" + modpath.string() + "] "
                "specifies a non existant language [" + lang + "]");

            break;
    }
}

const std::unique_ptr<module>& module_handler::find_module(const std::string& module_id) {
    try { return m_modules.at(module_id); }
    catch(const std::out_of_range&) {
        throw std::out_of_range("no module named " + module_id + " found");
    }
}

module_handler::language module_handler::parse_lang(const std::string& lang_string) {
    if(lang_string == "python") { return language::python; }
    else { return language::error; }
}