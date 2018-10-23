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
                    load_module(item.path(), js::parse(config_s));
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
            catch(js::parse_error& ex) {
                logger::log(logger::level::ERROR,
                    "failed to load module at [" + item.path().string() + "] "
                    "because the config was not a valid json. \n"
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
    const std::string& module_version,
    const std::string& category,
    const std::string& capability,
    js& args) {
        
    // put python module stuff in generic module
    try { return find_module(module_id, module_version)->pass_execution(category, capability, args); }
    catch(std::out_of_range& ex) {
        std::string capability_full = 
            module_id + "-" + module_version + "." + category + capability;

        std::string error = 
            "there was an attempt to execute capability [" + capability_full + "] "
            "but there was a broken link in the execution chain. "
            "please report this error to the servant authors if this keeps happening. "
            "maybe the module author forgot to change the module version? :P "
            "exception: " + ex.what();

        return result(result::type::ERROR, error);
    }
}

void module_handler::load_module(const fs::path& modpath, js& config_j) {
    // determine language of module
    js id_j       = config_j["id"];
    js lang_j     = config_j["lang"];
    js name_j     = config_j["name"];
    js author_j   = config_j["author"];
    js version_j  = config_j["version"];
    js modfile_j  = config_j["modfile"];

    // check if all json fields are valid strings
    // emphasis on the not operator :P
    if(! (lang_j.is_string() || modfile_j.is_string() || 
          name_j.is_string() || version_j.is_string() ||
          id_j.is_string()   || author_j.is_string())) {
           
        logger::log(logger::level::ERROR, 
            "the module at [" + modpath.string() + "] has an invalid config "
            "because one or more fields are invalid strings");

        return;
    }

    std::string id      = id_j.get<std::string>();
    std::string lang    = lang_j.get<std::string>();
    std::string name    = name_j.get<std::string>();
    std::string author  = author_j.get<std::string>();
    std::string version = version_j.get<std::string>();
    std::string modfile = modfile_j.get<std::string>();

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

const std::unique_ptr<module>& module_handler::find_module(
    const std::string& module_id, 
    const std::string& module_version) {

    try {
        auto& mod = m_modules.at(module_id);
        if(mod->version() != module_version)
            throw std::out_of_range("will be relaced lol");

        return mod;
    }
    catch(const std::out_of_range&) {
        throw std::out_of_range(
            "no module named " + module_id + " in version " + module_version + " found");
    }
}

const std::unique_ptr<module>& module_handler::find_module(const std::string& module_id) {
    return m_modules.at(module_id);
}

module_handler::language module_handler::parse_lang(std::string lang_string) {
    if(lang_string == "python") { return language::python; }
    else { return language::error; }
}