#include <iostream>
#include "logger.h"
#include "servant.h"

int main(int argc, char** argv) {
    std::wstring host_address(U("http://*:6969"));
    if(argc > 1) host_address = conversions::to_string_t(std::string(argv[1]));
    logger::log(logger::level::DEBUG, "waddup pimps");

    try {
        servant* s = new servant("modules", host_address);
        s->start();
        getchar();
        delete(s);
    }
    catch(const web::http::http_exception& ex) {
        logger::log(logger::level::ERROR, 
            std::string("failed to start webservice. details: ") + ex.what()
        );
    }
}
