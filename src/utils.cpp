// src/utils.cpp

#include "utils.h"
#include <algorithm>

LogLevel detect_log_level(const std::string& line, const LogLevelConfig& config)
{
    std::string lowerLine = to_lower(line);

    for (const auto& keyword : config.fatalKeywords) {
        if (lowerLine.find(keyword) != std::string::npos) {
            return LogLevel::FATAL;
        }
    }

    for (const auto& keyword : config.errorKeywords) {
        if (lowerLine.find(keyword) != std::string::npos) {
            return LogLevel::ERROR;
        }
    }

    for (const auto& keyword : config.warningKeywords) {
        if (lowerLine.find(keyword) != std::string::npos) {
            return LogLevel::WARNING;
        }
    }

    for (const auto& keyword : config.infoKeywords) {
        if (lowerLine.find(keyword) != std::string::npos) {
            return LogLevel::INFO;
        }
    }

    for (const auto& keyword : config.debugKeywords) {
        if (lowerLine.find(keyword) != std::string::npos) {
            return LogLevel::DEBUG;
        }
    }

    return LogLevel::UNKNOWN;
}


const char* get_log_level_color(LogLevel level)
{
    switch (level) {
        case LogLevel::FATAL:
            return RED_COLOR;
        case LogLevel::ERROR:
            return RED_COLOR;
        case LogLevel::WARNING:
            return YELLOW_COLOR;
        case LogLevel::INFO:
            return GREEN_COLOR;
        case LogLevel::DEBUG:
            return BLUE_COLOR;
        default:
            return RESET_COLOR;
    }
}

std::string to_lower(const std::string& str)
{
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    
    return result;
}