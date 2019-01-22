#ifndef MODULE_HANDLER_H
#define MODULE_HANDLER_H

#include <string>
#include <vector>
#include <fstream>
#include <streambuf>

#include "i_definable.h"
#include "python_module.h"
#include "python_capability.h"
#include "module.h"

namespace fs = std::filesystem;

class module_handler : public i_definable {
enum class language { error, python };
public:
    module_handler();

    void load_modules(const std::string& modules_path);
    result pass_execution(
        const std::string& module_id,
        const std::string& category,
        const std::string& capability,
        json::value& args);

    json::value define() override;

    // const ref to unique_ptr since we can't copy a unique_ptr
    // i mean .. thats kinds the point isn't it?
    const std::unique_ptr<module>& find_module(const std::string& module_id);
private:
    const std::string CONFIG_FILENAME{"config.json"};
    std::map<std::string, std::unique_ptr<module>> m_modules;
    //std::vector<std::unique_ptr<module>> m_modules;
    void load_module(const fs::path& modpath, json::value& config_j);
    language parse_lang(const std::string& lang_string);
};

#endif