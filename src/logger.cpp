#include "logger.h"
#include <iostream>

// create logger singleton
std::unique_ptr<logger> logger::m_logger = std::make_unique<logger>();

logger::logger() : m_level(level::DEBUG) { }
logger::~logger() { logger::log(level::DEBUG, "logger cleanup"); }

void logger::log(level log_level, const std::string& message) {
    // TODO: way more info, file logging, stderr
    if(log_level >= logger::m_logger->m_level) {
        std::cout << message << std::endl;
    }
}

void logger::log(level log_level, const std::wstring& message) {
    // TODO: way more info, file logging, stderr
    if(log_level >= logger::m_logger->m_level) {
        std::wcout << message << std::endl;
    }
}

void logger::set_level(level log_level) {
    logger::m_logger->m_level = log_level;
}