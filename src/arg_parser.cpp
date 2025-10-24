// src/arg_parser.cpp

#include "arg_parser.h"
#include "date.h"
#include "utils.h"
#include <stdexcept>
#include <regex>

ProgramOptions parse_arguments(int argc, char* argv[])
{
    if (argc <= MIN_REQUIRED_ARGS)
    {
        throw std::runtime_error("Usage: " + std::string(argv[0]) + 
                                " <input_file> <search_pattern1> [search_pattern2 ...] [-f/--log-format] [<log_format>] [-i] [-r] [-from <date>] [-to <date>]");
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

        else if (arg == "-f" || arg == "--log-format")
        {
            if (i + 1 >= argc)
            {
                throw std::runtime_error("Missing value after -f/--log-format flag.");
            }
            std::string formatStr = argv[++i];
            if (formatStr == "generic") options.logFormat = LogFormats::GENERIC;
            else if (formatStr == "syslog") options.logFormat = LogFormats::SYSLOG;
            else if (formatStr == "java") options.logFormat = LogFormats::JAVA;
            else if (formatStr == "android") options.logFormat = LogFormats::ANDROID;
            else
            {
                throw std::runtime_error("Unknown log format: " + formatStr);
            }
        }
        
        else if (arg == "-A" || arg == "--after-context")
        {
            if (i + 1 >= argc)
            {
                throw std::runtime_error("Missing value after -A/--after-context flag.");
            }

            try
            {
                options.afterContext = std::stoi(argv[++i]);
                if (options.afterContext < 0)
                {
                    throw std::runtime_error("After context (-A) value must be non-negative.");
                }
            }

            catch (const std::exception&)
            {
                throw std::runtime_error("Invalid integer value for -A flag: " + std::string(argv[i]));
            }
        }

        else if (arg == "-B" || arg == "--before-context")
        {
            if (i + 1 >= argc)
            {
                throw std::runtime_error("Missing value after -B/--before-context flag.");
            }

            try
            {
                options.beforeContext = std::stoi(argv[++i]);
                if (options.beforeContext < 0)
                {
                    throw std::runtime_error("Before context (-B) value must be non-negative.");
                }
            }

            catch (const std::exception&)
            {
                throw std::runtime_error("Invalid integer value for -B flag: " + std::string(argv[i]));
            }
        }

        else if (arg == "-C" || arg == "--context")
        {
            if (i + 1 >= argc)
            {
                throw std::runtime_error("Missing value after -C/--context flag.");
            }

            try
            {
                int contextValue {std::stoi(argv[++i])};
                if (contextValue < 0)
                {
                    throw std::runtime_error("Context (-C) value must be non-negative.");
                    if (contextValue < 0)
                    {
                        throw std::runtime_error("Context (-C) value must be non-negative.");
                    }
                    options.beforeContext = contextValue;
                    options.afterContext = contextValue;
                }
            }

            catch (const std::exception&)
            {
                throw std::runtime_error("Invalid integer value for -C flag: " + std::string(argv[i]));
            }
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

    // Optimization Update: Pre-detect date format from the log file
    options.detectedDateFormat = detect_date_format_from_file(options.inputFilePath);

    return options;
}