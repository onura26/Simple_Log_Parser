#include "utils.h"
#include <algorithm>

// ANSI color codes
const std::string RED_COLOR = "\033[31m";
const std::string YELLOW_COLOR = "\033[33m";
const std::string GREEN_COLOR = "\033[32m";
const std::string BLUE_COLOR = "\033[34m";
const std::string RESET_COLOR = "\033[0m";

std::string get_log_level_color(const std::string& line)
{
    if (line.find("error") != std::string::npos) { return RED_COLOR; }
    if (line.find("warning") != std::string::npos || line.find("warn") != std::string::npos) { return YELLOW_COLOR; }
    if (line.find("info") != std::string::npos) { return GREEN_COLOR; }
    if (line.find("debug") != std::string::npos) { return BLUE_COLOR; }
    
    return RESET_COLOR;
}

std::string to_lower(const std::string& str)
{
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    
    return result;
}