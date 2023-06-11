#include "tester.hpp"
#include <fstream>
#include <iostream>

void printUsage(char const* cmd)
{
    std::cout << "usage: " << cmd << " <output_filename>" << '\n'
              << "\t<output_filename> - filename to write results to."
              << '\n';
}

int main(int argc, char const** argv)
{
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }

    auto output_filename = argv[1];
    std::ofstream output_stream { output_filename, std::ios_base::out | std::ios_base::trunc };
    if (!output_stream.is_open()) {
        std::cout << "[Error] Failed to open output file!\n";
        return 1;
    }

    Tester tester {};
    tester.runTests(output_stream, 100);

    return 0;
}