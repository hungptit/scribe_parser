#include "fmt/format.h"
#include <string>

#include "ioutils.hpp"

#include "algorithms.hpp"
#include "line_parser.hpp"
#include <time.h>

int main(int argc, char *argv[]) {
    scribe::FileReader<1 << 16, scribe::LineStats> parser;
    scribe::LineStats stats;
    for (auto idx = 1; idx < argc; ++idx) { parser(argv[idx], stats); }
    stats.print("Summary:");
}
