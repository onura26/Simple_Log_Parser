// src/arg_parser.h

#ifndef ARG_PARSER_H
#define ARG_PARSER_H

#include <string>
#include <vector>
#include <optional>
#include <chrono>

constexpr int MIN_REQUIRED_ARGS {2};
constexpr int FIRST_PATTERN_ARG_INDEX {2};

struct ProgramOptions
{
    std::string inputFilePath;
    std::vector<std::string> searchPatterns;
    bool caseInsensitive {false};
    bool useRegex {false};
    
    // New fields for date range filtering
    std::optional<std::chrono::system_clock::time_point> fromTime;
    std::optional<std::chrono::system_clock::time_point> toTime;
};

ProgramOptions parse_arguments(int argc, char* argv[]);

#endif // ARG_PARSER_H