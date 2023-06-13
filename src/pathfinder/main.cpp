#include "tester.hpp"
#include <fstream>
#include <iostream>

void printUsage(char const* cmd, size_t tr_default, size_t er_default)
{
    std::cout << "usage: " << cmd << " <output_filename> [-tr=<test_repeat_count>] [-er=<endpoints_generation_repeat_count>]\n"
              << "\t<output_filename> - filename to write results to,\n"
              << "\t<test_repeat_count> - how many times each test should be repeated [default = " << tr_default << "],\n"
              << "\t<endpoints_generation_repeat_count> - how many times start and end points should be generated [default = " << er_default << "]\n"
              << "Increasing <test_repeat_count> and <endpoints_generation_repeat_count> may lead to more precise results,\n"
              << "however they are both performance-heavy"
              << '\n';
}

template <class T>
T parseArg(int argc, char const** argv, const std::string& arg_prefix, T const& default_value)
{
    for (int i = 0; i < argc; ++i) {
        std::string arg { argv[i] };
        if (arg.starts_with(arg_prefix)) {
            const std::string to_parse = arg.substr(arg_prefix.size());
            if (to_parse.empty()) {
                return default_value;
            }

            std::stringstream stream_to_parse { to_parse };
            T value;
            stream_to_parse >> value;

            if (stream_to_parse.fail()) {
                return default_value;
            }

            return value;
        }
    }

    return default_value;
}

int main(int argc, char const** argv)
{
    auto const test_repeat_count_default = 10uz;
    auto const endpoints_generation_repeat_count_default = 10uz;

    if (argc < 2) {
        printUsage(argv[0], test_repeat_count_default, endpoints_generation_repeat_count_default);
        return 1;
    }

    auto const test_repeat_count = parseArg(argc, argv, "-tr=", test_repeat_count_default);
    auto const endpoints_generation_repeat_count = parseArg(argc, argv, "-er=", endpoints_generation_repeat_count_default);
    auto const output_filename = argv[1];

    std::ofstream output_stream { output_filename, std::ios_base::out | std::ios_base::trunc };
    if (!output_stream.is_open()) {
        std::cout << "[Error] Failed to open output file!\n";
        return 1;
    }

    std::cout << "Starting with\n"
              << " - test_repeat_count = " << test_repeat_count << '\n'
              << " - endpoints_generation_repeat_count = " << endpoints_generation_repeat_count
              << '\n';

    Tester tester {};
    tester.runTests(output_stream, test_repeat_count, endpoints_generation_repeat_count);

    output_stream.close();
    return 0;
}