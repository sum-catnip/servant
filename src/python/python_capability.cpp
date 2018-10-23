#include "python_capability.h"
#include "python_category.h"
#include "python_module.h"

#include "logger.h"

python_capability::python_capability(
    category* category, 
    const std::string& name, 
    py::function capability)
    : capability(category, name), m_pycapability(capability) { }

result python_capability::execute(json& params) {
    if(params.size() != m_params.size()) {
        std::string error = 
            "internal error when trying to call capability " + m_fullname + ". "
            "capability parameter count is: [" + std::to_string(m_params.size()) + "] " 
            "given are: [" + std::to_string(params.size()) + "]. "
            "i bet the module author forgot to update the version number!";

        return result(result::type::ERROR, error);
    }

    // somehow need to check the function param count
    // if thats even possible in pybind..
    // maybe lets do an exec :/

    py::args args{};
    // parse all the params and transform them to python args
    for(int c = 0; c < params.size(); c++ /* pun intended */) {
        // this will throw an exception if it failed to parse
        // so ill need to catch it ^^
        // knowing myself i will pops forget tho
        args[c] = m_params[c]->parse(params[c]);
    }
    
    // needed in outer scope for error handling
    py::object python_result;
    try {
        python_result = m_pycapability(args);
        // return value should be a of type result but no worries
        return python_result.cast<result>();
    }
    catch(py::error_already_set& ex) { // capability threw exception
        std::string error = "capability [" + m_fullname + "] threw an exception: " + ex.what();
        return result(result::type::ERROR, error);
    }
    catch(py::cast_error&) { // returned value was not of type result
        logger::log(logger::level::WARNING, 
            "capability [" + m_fullname + "] returned an object that was not of type result. "
            "this should be fine as long as it's not an error. returned obj: " + 
            static_cast<std::string>(py::str(python_result)));

        if(python_result.is_none()) { 
            return result(result::type::OK, "no additional information"); }

        // just return the returned object as a string as the last straw
        return result(result::type::OK, py::str(python_result));
    }
}
