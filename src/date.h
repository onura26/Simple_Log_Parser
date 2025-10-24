// src/date.h
#ifndef DATE_H
#define DATE_H

#include <string>
#include <optional>
#include <chrono>

enum class LogDateFormat 
{
    YYYY_MM_DD_HH_MM_SS,
    DD_MM_YYYY_HH_MM_SS,
    MM_DD_YYYY_HH_MM_SS,
    UNKNOWN
};

// Constant(s)
constexpr size_t TIMESTAMP_PREFIX_LENGTH = 19; // Length of "YYYY-MM-DD HH:MM:SS"

// Function Declarations
LogDateFormat detect_date_format(const std::string& dateStr);
std::optional<std::chrono::system_clock::time_point> parse_log_timestamp(
    const std::string& dateStr, 
    LogDateFormat format);
std::optional<std::chrono::system_clock::time_point> extract_timestamp(const std::string& line);

#endif // DATE_H