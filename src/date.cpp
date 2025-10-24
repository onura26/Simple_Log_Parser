// src/date.cpp
#include "date.h"
#include <sstream>
#include <iomanip>
#include <regex>
#include <ctime>

// Detect the date format based on the input string
LogDateFormat detect_date_format(const std::string& dateStr)
{
    // Regex patterns for different date formats defined in enum LogDateFormat
    static const std::regex ymd(R"(^\d{4}[-./]\d{2}[-./]\d{2})");
    static const std::regex dmy(R"(^\d{2}[-./]\d{2}[-./]\d{4})");
    static const std::regex mdy(R"(^\d{2}[-./]\d{2}[-./]\d{4})");

    // Check for patterns, return corresponding enum value
    if (std::regex_search(dateStr, ymd))
        return LogDateFormat::YYYY_MM_DD_HH_MM_SS;
    if (std::regex_search(dateStr, dmy))
        return LogDateFormat::DD_MM_YYYY_HH_MM_SS;
    if (std::regex_search(dateStr, mdy))
        return LogDateFormat::MM_DD_YYYY_HH_MM_SS;
    
    return LogDateFormat::UNKNOWN; // Unknown or unsupported format
}

// Parse log timestamp based on detected format
std::optional<std::chrono::system_clock::time_point> parse_log_timestamp(const std::string& dateStr, LogDateFormat format)
{
    // Initialize a tm structure to hold parsed time components
    std::tm tm = {};
    std::istringstream ss(dateStr); // Input string stream for parsing

    switch (format)
    {
        case LogDateFormat::YYYY_MM_DD_HH_MM_SS:
            ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
            break;
        case LogDateFormat::DD_MM_YYYY_HH_MM_SS:
            ss >> std::get_time(&tm, "%d-%m-%Y %H:%M:%S");
            break;
        case LogDateFormat::MM_DD_YYYY_HH_MM_SS:
            ss >> std::get_time(&tm, "%m-%d-%Y %H:%M:%S");
            break;
        default:
            return std::nullopt; // Unsupported format
    }

    // Check if parsing was successful
    if (ss.fail())
        return std::nullopt;
    
    // tm -> time_t conversion and then to time_point
    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

// Extract timestamp from a log line
std::optional<std::chrono::system_clock::time_point> extract_timestamp(const std::string& line)
{
    // Log timestamp length check
    if (line.size() < TIMESTAMP_PREFIX_LENGTH)
        return std::nullopt; 

    std::string prefix = line.substr(0, TIMESTAMP_PREFIX_LENGTH); // 
    LogDateFormat format = detect_date_format(prefix);
    
    return parse_log_timestamp(prefix, format); // Call parsing function
}


