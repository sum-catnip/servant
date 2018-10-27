

#include "python_capability.h"
#include "python_category.h"
#include "python_module.h"

#include "logger.h"



python_capability::python_capability( 
    const std::string& name,
    py::function capability)
    : capability(name), m_pycapability(capability) { }

python_capability::python_capability(
    const std::string& name, 
    py::function capability,
    const std::vector<std::shared_ptr<parameter>> params)
    : capability(name, params), m_pycapability(capability) { }

result python_capability::execute(json::value& params) {
    if(params.size() != m_params.size()) return result(result::type::ERROR, 
        "internal error when trying to call capability " + fullname() + ". "
        "capability parameter count is: [" + std::to_string(m_params.size()) + "] " 
        "given are: [" + std::to_string(params.size()) + "]. "
        "i bet the module author forgot to update the version number!");

    // somehow need to check the function param count
    // if thats even possible in pybind..
    // maybe i'll do an exec :/

    py::list list{};

    // needed in outer scope for error handling
    py::object python_result;
    try {
        for(int c = 0; c < params.size(); c++ /* pun intended */) {
            // this will throw an exception if it failed to parse
            // so ill need to catch it ^^
            // knowing myself i will props forget tho
            list.append(m_params[c]->parse(params.as_array()[c]));
        }

        python_result = m_pycapability(*list);
        // return value should be a of type result but no worries
        return python_result.cast<result>();
    }
    catch(py::error_already_set& ex) { // capability threw exception
        std::string error = "capability [" + fullname() + "] threw an exception: " + ex.what();
        return result(result::type::ERROR, error);
    }
    catch(py::cast_error&) { // returned value was not of type result
        logger::log(logger::level::WARNING, 
            "capability [" + fullname() + "] returned an object that was not of type result. "
            "this should be fine as long as it's not an error. returned obj: " + 
            static_cast<std::string>(py::str(python_result)));

        if(python_result.is_none()) { 
            return result(result::type::OK, "no additional information"); }

        // just return the returned object as a string as the last straw
        return result(result::type::OK, py::str(python_result));
    }
}
