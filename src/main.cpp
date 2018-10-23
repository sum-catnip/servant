#include <iostream>
#include "logger.h"
#include "servant.h"

int main(int argc, char** argv) {
    logger::log(logger::level::DEBUG, "waddup pimps");

    servant* s = new servant("../../modules", U("http://localhost:6969"));
    s->start();

    getchar();

    delete(s);
}
