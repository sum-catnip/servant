#include "python_module.h"
#include "logger.h"

#include "python_bindings.h"

python_module::python_module(
    const std::string& path, 
    const std::string& name,
    const std::string& id,
    const std::string& version,
    const std::string& author,
    const std::string& module)
    : module(path, name, id, version, author, module) {
    
    load_module();
}

void python_module::load_module() {
    try {
        // this should exist over the lifespan of the program
        static py::scoped_interpreter interpreter{};
        static py::gil_scoped_release release_gil{};

        try {
            py::gil_scoped_acquire gil{};
            gil.inc_ref();

            py::module::import("servant");
            py::module main  = py::module::import("__main__");
            py::dict globals = main.attr("__dict__");

            py::dict locals;
            locals["module_name"] = m_id;
            locals["path"]        = m_modulefile;
            locals["module"]      = py::cast(this);

            py::eval<py::eval_statements>(
                "import importlib.util\n"
                "spec  = importlib.util.spec_from_file_location(module_name, path)\n"
                "pymod = importlib.util.module_from_spec(spec)\n"
                "pymod.__dict__['__module__'] = module\n"
                "spec.loader.exec_module(pymod)",
                globals,
                locals);

            py::module mod = locals["pymod"];
            m_pymod = mod;
        }
        catch(py::error_already_set& ex) {
            logger::log(logger::level::ERROR,
                m_name + " threw an unhandled exception: \n" + 
                ex.what()
            );
        }
        //PyThreadState* state = PyEval_SaveThread();
    }
    catch(std::exception) {
        logger::log(logger::level::ERROR, "something strange happened...");
    }
}