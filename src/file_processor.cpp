// src/file_processor.cpp

#include "file_processor.h"
#include "utils.h"
#include "date.h"
#include <iostream>
#include <fstream>
#include <regex>
#include <cstdlib>

int search_in_file(const ProgramOptions& options)
{
    /*
    * CONTEXT LINES ALGORITHM 
    * 
    * This function implements grep-style context lines (-A, -B, -C flags).
    * 
    * 1. Ring Buffer (Before-Context):
    *   - Continously stores the last N lines in a deque (instead of vector for faster operations)
    *   - When match found -> dump buffer, then clear it
    *   - Deque has O(1) for push_back/pop_front 
    * 
    * 2. Countdown Timer (After-Context):
    *   - After a match, print next N lines regardless of pattern
    *   - Decrements counter each line until it reaches zero
    * 
    * 3. Deduplication:
    *   - Tracks last printed line number to avoid duplicates
    *   - Handles overlapping contexts when matches are close together
    * 
    * 4. Separators:
    *   - Prints "--" between close match groups (just like grep)
    * 
    * Ex:
    *   ./logparser log.txt "ERROR" -B 2 -A 1
    * 
    *   Output:
    *   [C:L18] line before match     ← before context (dim)
    *   [C:L19] line before match     ← before context (dim)
    *   [0:L20] ERROR: actual match   ← match (colored)
    *   [C:L21] line after match      ← after context (dim)
    *   --
    *   [C:L45] line before match     ← next match group
    *   [1:L46] ERROR: another match
    */

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

    // Context lines implementation
    // Ring buffer for before-context lines (-B flag)
    std::deque<std::pair<int, std::string>> beforeBuffer;

    int afterContextRemaining {0}; // Countdown timer for after-context lines (-A flag)

    int lastPrintedLine {-1}; // Deduplication tracker that prevents printing the same line twice

    bool needsSeparator {false}; // Separator flag 

    std::string line;
    int matchCount = 0;
    int lineNumber = 0;
    int linesWithTimestamps = 0;

    // Optimization Update: Use pre-detected date format
    LogDateFormat dateFormat = options.detectedDateFormat;

    constexpr const char* CONTEXT_COLOR = "\033[2m]"; // dim 

    while (std::getline(inputFile, line))
    {
        ++lineNumber;

        // Date range filtering
        auto ts = extract_timestamp(line, dateFormat);
        if (ts)
        {
            ++linesWithTimestamps;
            if (options.fromTime && *ts < *(options.fromTime))
            {
                continue; // Skip lines before fromTime
            }
            if (options.toTime && *ts > *(options.toTime))
            {
                continue; // Skip lines after toTime
            }
        }

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
            // Step 1: Print separator between non-contigous matches
            // Ex: Match at line 10, last printed line was 7, need separator
            if (needsSeparator && lastPrintedLine != -1 && lineNumber - lastPrintedLine > 1)
            {
                std::cout << "--\n";
            }

            // Step 2: Dump ring buffer (before context)
            // Loop through stored lines in beforeBuffer
            for (const auto& [bufLineNum, bufLine] : beforeBuffer)
            {
                // Deduplication check
                // If matches are close, avoid re-printing same context lines
                if (bufLineNum > lastPrintedLine)
                {
                    // Ex: lastPrintedLine = 10, bufLineNum = 11 -> bufLineNum annexes lastPrintedLine after it was printed
                    std::cout << CONTEXT_COLOR << "[C:L" << bufLineNum << "] " << bufLine << RESET_COLOR << '\n';
                    lastPrintedLine = bufLineNum;
                }
            }

            // Step 3: Print the actual matching line (colored by log level)
            LogLevel level = detect_log_level(line, options.logFormat);
            auto color = get_log_level_color(level);
            std::cout << color << "[" << matchCount << ":L" << lineNumber <<"] " << line << RESET_COLOR << '\n';
            lastPrintedLine = lineNumber;
            ++matchCount;

            // Step 4: Set after context counter
            // Set counter to print next N lines as context after the match
            afterContextRemaining = options.afterContext;

            // Step 5: Clear before buffer
            // Since they are printed, we won't need them anymore. Let's kill them!
            beforeBuffer.clear();

            // Step 6: Reset separator flag, since we might have more matches right after
            needsSeparator = true;
        }

        else if (afterContextRemaining > 0)
        {
            // After context processing
            if (lineNumber > lastPrintedLine) // Deduplication check
            {
                std::cout << CONTEXT_COLOR << "[C:L" << lineNumber << "] " << line << RESET_COLOR << '\n';
                lastPrintedLine = lineNumber;
            }
            --afterContextRemaining; // Decrement counter 
        }

        else
        {
            // Store line in before buffer
            if (options.beforeContext > 0)
            {
                beforeBuffer.push_back({lineNumber, line}); // O(1)
                
                // Maintain buffer size (sliding window)
                // Ex: buffer size=3, we have 4 lines, pop the oldest (front)
                // [line10, line11, line12] + line13 -> [line11, line12, line13]
                if (static_cast<int>(beforeBuffer.size()) > options.beforeContext)
                {
                    beforeBuffer.pop_front(); // O(1)
                }
            }
        }
    }

    // Warn user if date filtering was applied but no timestamps were found
    if ((options.fromTime || options.toTime) && linesWithTimestamps == 0)
    {
        std::cerr << "\nWarning: Date filtering was requested, but no valid timestamps were found in the log lines.\n";
    }

    std::cout << "\nTotal Matches: " << matchCount << std::endl;

    return EXIT_SUCCESS;
}