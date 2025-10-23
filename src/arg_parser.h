// src/arg_parser.h

#ifndef ARG_PARSER_H
#define ARG_PARSER_H

#include <string>
#include <vector>

constexpr int MIN_REQUIRED_ARGS {2};
constexpr int FIRST_PATTERN_ARG_INDEX {2};

struct ProgramOptions
{
    std::string inputFilePath;
    std::vector<std::string> searchPatterns;
    bool caseInsensitive {false};
    bool useRegex {false};
};

ProgramOptions parse_arguments(int argc, char* argv[]);

#endif // ARG_PARSER_H