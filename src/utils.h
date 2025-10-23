#ifndef UTILS_H
#define UTILS_H

#include <string>

// ANSI color codes for terminal text formatting
extern const std::string RED_COLOR;
extern const std::string YELLOW_COLOR;
extern const std::string GREEN_COLOR;
extern const std::string BLUE_COLOR;
extern const std::string RESET_COLOR;

// Utility Functions
std::string get_log_level_color(const std::string& line);
std::string to_lower(const std::string& str);

#endif // UTILS_H