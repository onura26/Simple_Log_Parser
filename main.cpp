#include <iostream>
#include <cstdlib>
#include "src/arg_parser.h"
#include "src/file_processor.h"

int main(int argc, char* argv[])
{
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