// src/arg_parser.cpp

#include "arg_parser.h"
#include "date.h"
#include <stdexcept>
#include <regex>

ProgramOptions parse_arguments(int argc, char* argv[])
{
    if (argc <= MIN_REQUIRED_ARGS)
    {
        throw std::runtime_error("Usage: " + std::string(argv[0]) + 
                                " <input_file> <search_pattern1> [search_pattern2 ...] [-i] [-r] [-from <date>] [-to <date>]");
    }
    
    ProgramOptions options;
    options.inputFilePath = argv[1];
    options.caseInsensitive = false;
    options.useRegex = false;

    for (int i = FIRST_PATTERN_ARG_INDEX; i < argc; ++i)
    {
        std::string arg = argv[i];

        if (arg == "-i")
        {
            options.caseInsensitive = true;
        }

        else if (arg == "-r")
        {
            options.useRegex = true;
        }

        else if (arg == "-from")
        {
            if (i + 1 >= argc)
            {
                throw std::runtime_error("Missing value after -from flag.");
            }
            std::string dateStr = argv[++i];
            auto frmt = detect_date_format(dateStr);
            auto parsed = parse_log_timestamp(dateStr, frmt);
            if (!parsed)
            {
                throw std::runtime_error("Invalid date format for -from: " + dateStr);
            }
            options.fromTime = parsed;
        }

        else if (arg == "-to")
        {
            if (i + 1 >= argc)
            {
                throw std::runtime_error("Missing value after -to flag.");
            }
            std::string dateStr = argv[++i];
            auto frmt = detect_date_format(dateStr);
            auto parsed = parse_log_timestamp(dateStr, frmt);
            if (!parsed)
            {
                throw std::runtime_error("Invalid date format for -to: " + dateStr);
            }
            options.toTime = parsed;
        }

        else
        {
            options.searchPatterns.push_back(arg);
        }
    }

    if (options.searchPatterns.empty())
    {
        throw std::runtime_error("No search pattern(s) provided. At least one pattern is required.");
    }

    // Regex pattern validation
    if (options.useRegex)
    {
        for (const auto& pattern : options.searchPatterns)
        {
            try
            {
                std::regex testRegex(pattern, options.caseInsensitive ? std::regex::icase : std::regex::ECMAScript);
            }
            catch (const std::regex_error& e)
            {
                throw std::runtime_error("Invalid regex pattern: '" + pattern + "': " + e.what());
            }
        }
    }

    return options;
}