// src/date.cpp
#include "date.h"
#include <sstream>
#include <iomanip>
#include <regex>
#include <ctime>
#include <fstream>

// Detect the date format based on the input string
LogDateFormat detect_date_format(const std::string& dateStr)
{
    // Regex patterns for different date formats defined in enum LogDateFormat
    // NOTE: Check YYYY-MM-DD first (most common for logs)
    static const std::regex ymd(R"(^\d{4}[-./]\d{2}[-./]\d{2})");

    if (std::regex_search(dateStr, ymd))
        return LogDateFormat::YYYY_MM_DD_HH_MM_SS;

    // For DD-MM-YYYY vs MM-DD-YYYY
    // We will default to DD-MM-YYYY (European/ISO standard)
    // If you need MM-DD-YYYY, change the order or add a config flag
    static const std::regex dmyOrMdy(R"(^\d{2}[-./]\d{2}[-./]\d{4})");

    if (std::regex_search(dateStr, dmyOrMdy))
    {
        // Try parsing as DD-MM-YYYY first
        // If your logs are US format, return MM-DD-YYYY instead
        return LogDateFormat::DD_MM_YYYY_HH_MM_SS;
    }

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
    
    // Fix: UTC compatibility
    tm.tm_isdst = -1; // Let mktime determine if DST is in effect
    
    // tm -> time_t conversion and then to time_point
    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

// Extract timestamp from a log line using pre-detected format
std::optional<std::chrono::system_clock::time_point> extract_timestamp(const std::string& line, LogDateFormat format)
{
    // Log timestamp length check
    if (line.size() < TIMESTAMP_PREFIX_LENGTH)
        return std::nullopt; 

    std::string prefix = line.substr(0, TIMESTAMP_PREFIX_LENGTH); // 
    
    // For performance, no more regex detection here - just parse with know format.

    return parse_log_timestamp(prefix, format); // Call parsing function
}

// New Function: Detect date format from a log file by reading the first few lines
LogDateFormat detect_date_format_from_file(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
        return LogDateFormat::UNKNOWN;

    std::string line;

    // Read first few lines to detect format
    for (int i = 0; i < 100 && std::getline(file, line); ++i)
    {
        if (line.size() >= TIMESTAMP_PREFIX_LENGTH)
        {
            std::string prefix = line.substr(0, TIMESTAMP_PREFIX_LENGTH);
            LogDateFormat format = detect_date_format(prefix);

            if (format != LogDateFormat::UNKNOWN)
                return format; // Return the first detected format
        }
    }

    return LogDateFormat::UNKNOWN; 
}
