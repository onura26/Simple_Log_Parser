// main.cpp

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdlib>

// Constants
constexpr int MIN_REQUIRED_ARGS = 2; // Minimum required arguments: filename and at least one search pattern
constexpr int FIRST_PATTERN_ARG_INDEX = 2; // Index of the first search pattern argument

// ANSI color codes for terminal output
const std::string RED_COLOR = "\033[1;31m";
const std::string YELLOW_COLOR = "\033[1;33m";
const std::string GREEN_COLOR = "\033[1;32m";
const std::string BLUE_COLOR = "\033[1;34m";
const std::string RESET_COLOR = "\033[0m";

struct ProgramOptions
{
    std::string inputFilePath;
    std::vector<std::string> searchPatterns;
    bool caseInsensitive = false;
};

// Function Declarations
ProgramOptions parse_arguments(int argc, char* argv[]);
std::string get_log_level_color(const std::string& line);
std::string to_lower(const std::string& str);
void process_line_and_count(const std::string& line, const std::vector<std::string>& patterns, 
                            bool caseInsensitive, int& matchCount);
int search_in_file(const ProgramOptions& options);

int main(int argc, char* argv[]) {
    
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    try
    {
        ProgramOptions options = parse_arguments(argc, argv);
        return search_in_file(options);
    }
    
    catch (const std::exception& ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}

// Function Definitions
ProgramOptions parse_arguments(int argc, char* argv[])
{
    if (argc <= MIN_REQUIRED_ARGS)
    {
        throw std::runtime_error("Usage: " + std::string(argv[0]) + " <input_file> <search_pattern1> [search_pattern2 ...] [-i]");
    }

    ProgramOptions options;
    options.inputFilePath = argv[1];
    options.caseInsensitive = false;

    for (int i = FIRST_PATTERN_ARG_INDEX; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "-i")
        {
            options.caseInsensitive = true;
        }
        else
        {
            options.searchPatterns.push_back(arg);
        }
    }

    if (options.caseInsensitive)
    {
        for (auto& pattern : options.searchPatterns)
        {
            pattern = to_lower(pattern);
        }
    }

    if (options.searchPatterns.empty())
    {
        throw std::runtime_error("No search pattern(s) provided. At least one pattern is required.");
    }

    return options;
}

int search_in_file(const ProgramOptions& options)
{
    std::ifstream inputFile(options.inputFilePath);
    if (!inputFile.is_open())
    {
        throw std::runtime_error("Failed to open file: " + options.inputFilePath);
    }

    std::string line;
    int matchCount = 0;
    
    while (std::getline(inputFile, line))
    {
        process_line_and_count(line, options.searchPatterns, options.caseInsensitive, matchCount);
    }

    std::cout << "\nTotal matches: " << matchCount << " lines" << std::endl;

    return EXIT_SUCCESS;
}

void process_line_and_count(const std::string& line, const std::vector<std::string>& patterns, 
                    bool caseInsensitive, int& matchCount)
{
    const std::string lowerLine = caseInsensitive ? to_lower(line) : line;

    // Loop through each pattern to check for a match
    for (const auto& pattern : patterns)
    {
        bool found = (lowerLine.find(pattern) != std::string::npos);

        if (found)
        {
            // Get color based on log level
            std::string color = get_log_level_color(lowerLine);
            ++matchCount;
            std::cout << color << "[" << matchCount << "] " << line << RESET_COLOR << '\n';
            return; // Avoid counting the same line multiple times
        }
    }
}

// Helper Functions
std::string get_log_level_color(const std::string& line)
{
    // Check once, return early
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
