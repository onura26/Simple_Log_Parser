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

    while (std::getline(inputFile, line))
    {
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
            // Simple string search
            const std::string searchLine = options.caseInsensitive ? to_lower(line) : line;

            for (const auto& pattern : options.searchPatterns)
            {
                const std::string searchPattern = options.caseInsensitive ? to_lower(pattern) : pattern;

                if (searchLine.find(searchPattern) != std::string::npos)
                {
                    found = true;
                    break;
                }
            }
        }

        if (found)
        {
            ++matchCount;
            std::string color = get_log_level_color(to_lower(line));
            std::cout << color << "[" << matchCount << "] " << line << RESET_COLOR << '\n';
        }
    }

    std::cout << "\nTotal Matches: " << matchCount << std::endl;

    return EXIT_SUCCESS;
}