// src/file_processor.cpp

#include "file_processor.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <regex>
#include <cstdlib>

int search_in_file(const ProgramOptions& options)
{
    std::ifstream inputFile(options.inputFilePath);

    if (!inputFile.is_open())
    {
        throw std::runtime_error("Failed to open file: " + options.inputFilePath);
    }

    // Precompute lowercase patterns if case insensitive
    std::vector<std::string> lowerCasePatterns;
    if (options.caseInsensitive && !options.useRegex)
    {
        lowerCasePatterns.reserve(options.searchPatterns.size());

        for (const auto& pattern : options.searchPatterns)
        {
            lowerCasePatterns.push_back(to_lower(pattern));
        }
    }

    // Compile regex patterns if needed
    std::vector<std::regex> regexPatterns;
    if (options.useRegex)
    {
        for (const auto& pattern : options.searchPatterns)
        {
            regexPatterns.emplace_back(pattern, options.caseInsensitive ? std::regex::icase : std::regex::ECMAScript);
        }
    }

    std::string line;
    int matchCount = 0;
    int lineNumber = 0;

    while (std::getline(inputFile, line))
    {
        ++lineNumber;
        bool found = false;

        if (options.useRegex)
        {
            // Use regex search
            for (const auto& regexPattern : regexPatterns)
            {
                if (std::regex_search(line, regexPattern))
                {
                    found = true;
                    break;
                }
            }
        }

        else
        {
            const std::string searchLine = options.caseInsensitive ? to_lower(line) : line;
            const auto& patternsToUse = options.caseInsensitive ? lowerCasePatterns : options.searchPatterns;

            for (const auto& pattern : patternsToUse)
            {
                if (searchLine.find(pattern) != std::string::npos)
                {
                    found = true;
                    break;
                }
            }
        }

        if (found)
        {
            std::string color = get_log_level_color(to_lower(line));
            std::cout << color << "[" << matchCount << ":L" << lineNumber <<"] " << line << RESET_COLOR << '\n';
            ++matchCount;
        }
    }

    std::cout << "\nTotal Matches: " << matchCount << std::endl;

    return EXIT_SUCCESS;
}