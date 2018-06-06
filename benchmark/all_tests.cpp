#include "celero/Celero.h"

#include "fmt/format.h"
#include "grep.hpp"
#include "ioutils/ioutils.hpp"
#include <algorithm>
#include <iostream>
#include <string>

constexpr int number_of_samples = 10;
constexpr int number_of_operations = 1;

CELERO_MAIN

const std::string datafile("3200.txt");
const std::vector<std::string> patterns = {"Twain",
                                           "(?i)Twain",
                                           "[a-z]shing",
                                           "Huck[a-zA-Z]+|Saw[a-zA-Z]+",
                                           "\b\\w+nn\b",
                                           "[a-q][^u-z]{13}x",
                                           "Tom|Sawyer|Huckleberry|Finn"};

namespace {
    void run_a_test(const std::string datafile, const std::string &command,
                    const std::string &pattern) {
        std::string cmd = command + " '" + pattern + "' " + datafile + " | wc > /dev/null";
        system(cmd.c_str());
    }
    void run_all_tests(const std::string datafile, const std::string &command,
                       const std::vector<std::string> &patterns) {
        for (auto const &item : patterns) { run_a_test(datafile, command, item); }
    }
} // namespace

BASELINE(mark_twain, grep_brew, number_of_samples, number_of_operations) {
    run_all_tests(datafile, "grep -E", patterns);
}

BENCHMARK(mark_twain, ag, number_of_samples, number_of_operations) {
    run_all_tests(datafile, "ag", patterns);
}

BENCHMARK(mark_twain, ripgrep, number_of_samples, number_of_operations) {
    run_all_tests(datafile, "rg", patterns);
}

BENCHMARK(mark_twain, fgrep, number_of_samples, number_of_operations) {
    run_all_tests(datafile, "../commands/fgrep", patterns);
}

BENCHMARK(mark_twain, fastgrep, number_of_samples, number_of_operations) {
    run_all_tests(datafile, "../commands/fastgrep", patterns);
}
