#ifndef LOGGER_H
#define LOGGER_H

#include <string>

// singleton logger implementation
class logger {
public:
    enum class level {DEBUG, INFO, WARNING, ERROR};

    static void log(level log_level, const std::string& message);
    static void log(level log_level, const std::wstring& message);
    static void set_level(level log_level);

    ~logger();
    logger();
private:
    static std::unique_ptr<logger> m_logger;

    level m_level;
    // TODO: file logging members
};

#endif