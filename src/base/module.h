#ifndef MODULE_H
#define MODULE_H

#include <string>

#include "i_definable.h"
#include "category.h"
#include "capability.h"

// abstract base class for language specific modules
class module : public i_definable {
public:
    std::string fullname();
    std::string name();
    std::string version();

    result pass_execution(
        const std::string& category, 
        const std::string& capability,
        json::value& args);

    virtual json::value define() override;

    void add_category(const std::shared_ptr<category> category);
    std::shared_ptr<category> find_category(const std::string category);
protected:

    module(
        const std::string& path, 
        const std::string& name,
        const std::string& id,
        const std::string& version,
        const std::string& author,  
        const std::string& module);

    virtual void load_module() = 0;

    std::map<std::string, std::shared_ptr<category>> m_categories;

    // the folder containing the module
    std::string m_path;

    // the display name of the module (can contain whitespace and stuff)
    std::string m_name;

    // the internal id/name of the module (restricted characters)
    std::string m_id;

    // the version (?number) of the module
    std::string m_version;

    // the author(s) of the module
    std::string m_author;

    // the filename/path of the main module file
    std::string m_modulefile;    
};

#endif