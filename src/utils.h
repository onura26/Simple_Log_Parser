// src/utils.h

#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

// ANSI color codes for terminal text formatting
constexpr const char* RED_COLOR = "\033[31m";
constexpr const char* YELLOW_COLOR = "\033[33m";
constexpr const char* GREEN_COLOR = "\033[32m";
constexpr const char* BLUE_COLOR = "\033[34m";
constexpr const char* RESET_COLOR = "\033[0m";

enum class LogLevel {
    FATAL,
    ERROR,
    WARNING,
    INFO,
    DEBUG,
    UNKNOWN
};

// Define the struct FIRST, before using it
struct LogLevelConfig {
    std::vector<std::string> fatalKeywords;
    std::vector<std::string> errorKeywords;
    std::vector<std::string> warningKeywords;
    std::vector<std::string> infoKeywords;
    std::vector<std::string> debugKeywords;
};

// NOW you can use it in the namespace
namespace LogFormats {
    inline const LogLevelConfig GENERIC = {
        {"fatal", "critical", "emergency", "alert"},
        {"error", "err", "exception"},
        {"warning", "warn", "caution"},
        {"info", "information", "notice"},
        {"debug", "dbg", "trace", "verbose"}
    };

    inline const LogLevelConfig SYSLOG = {
        {"emergency", "alert", "critical"},
        {"error"},
        {"warning", "notice"},
        {"info"},
        {"debug"}
    };

    inline const LogLevelConfig JAVA = {
        {"fatal"},
        {"error"},
        {"warn"},
        {"info"},
        {"debug", "trace"}
    };

    inline const LogLevelConfig ANDROID = {
        {" F ", " F/"}, 
    {" E ", " E/"},
    {" W ", " W/"},
    {" I ", " I/"},
    {" D ", " V "}  // V = verbose = debug level
    };
}

inline const LogLevelConfig& DEFAULT_LOG_LEVEL_CONFIG = LogFormats::GENERIC;

// Utility Functions
LogLevel detect_log_level(const std::string& line, const LogLevelConfig& config);
const char* get_log_level_color(LogLevel level);
std::string to_lower(const std::string& str);

#endif // UTILS_H