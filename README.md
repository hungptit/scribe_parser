# Introduction
The fgrep utility searches any given input files for lines that match one or more patterns. fgrep is written using modern C++ and is built on top of [ioutils](https://github.com/hungptit/ioutils "A blazing fast fast file I/O library"), [utils](https://github.com/hungptit/utils "A very fast string related functionality written in modern C++"), [fmt](https://github.com/fmtlib/fmt "A modern formating library"), [cereal](https://github.com/USCiLab/cereal "A C++11 library for serialization"), [hyperscan](https://github.com/intel/hyperscan "High-performance regular expression matching library."), and [boost libraries](https://www.boost.org/ "boost.org"). Our benchmarks and unit tests are written using Google [benchmark](https://github.com/google/benchmark "A microbenchmark support library"), [Celero](https://github.com/DigitalInBlue/Celero "C++ Benchmark Authoring Library/Framework"), and [Catch2](https://github.com/catchorg/Catch2 "A modern, C++-native, header-only, test framework for unit-tests, TDD and BDD").

# What is the different between fgrep and grep or ripgrep #

fgrep is modular and it can be reused in other projects. All core algorithms are templatized so we can have flexible and reusable code without sacrificing the performance.

# Why fgrep is fast? #

The fgrep command is fast because of many reasons and below are key factors

## High-performance regular rexpression matching engine ##

fgrep uses [hyperscan](https://github.com/intel/hyperscan) as a regular expression matching engine. Our performance benchmarks have shown that [hyperscan](https://github.com/intel/hyperscan) is 20x or more faster than that of std::regex.

## High-performance exact text matching algorithm ##

fgrep uses SSE2 and AVX2 optimized algorithms for exact text matching. Our SSE2 and AVX2 algorithms are forked from [this repository](https://github.com/WojciechMula/sse4-strstr). Our exact matching algorithms is 2-4x faster than the standard string find algorithm.

## Efficient file I/O ##

fgrep uses very fast algorithms for reading data from file.

## Modern C++ ##

All core algorithms are implemented using [Policy Based Design](https://en.wikipedia.org/wiki/Policy-based_design). This approach allows compiler to generated classes at compile time and it also provides an opportunity for compiler to inline methods and functions at compile time.

# Benchmark results #

It is impossible to get a good benchmark for text searching tools since the performance of each benchmarked command is relied on search patterns. We use all patterns mentioned in this [link](https://rust-leipzig.github.io/regex/2017/03/28/comparison-of-regex-engines/) and their test data. All benchmarks can be found from the [benchmark folder](https://github.com/hungptit/fastgrep/tree/master/benchmark) and our benchmark results are consistent in all test platforms such as Gentoo kernel 4.17, CentOS 6.x, and macOS.

## Test environments ##
**Linux**
* CPU: * Intel(R) Xeon(R) CPU E5-2699 v4 @ 2.20GHz
* System memory: 792 GBytes
* Hard drive: Very fast network drives
* OS: Kernel 3.8.13
* Compiler: gcc-5.5.0

**MacBook Pro**
* CPU: 2.2 GHz Intel Core i7
* System memory: 16 GBytes
* Hard drive: SSD
* OS: Darwin Kernel Version 16.7.0
* Compiler: Apple LLVM version 9.0.0 (clang-900.0.39.2)

## Test data ##

Test data and patterns are obtained from this [article](https://rust-leipzig.github.io/regex/2017/03/28/comparison-of-regex-engines/). Benchmark results show that:
* fastgrep and ripgrep performance are comparable.
* fastgrep outperform grep in term of performance especially for complicated regular expression patterns. 
* ag is the slower than grep, ripgrep, and fastgrep. *Beside that ag cannot handle files that are bigger than 2GB.*

## Experiment setup ##

* All tests command are run using default options.
* The output of all commands are redirected to a temporary file.

## Results ##
**Linux**

Below benchmark results are collected with 
* GNU grep 3.1
* ag 2.2.0
* rg 0.10.0
* fgrep master branch.

```
./all_tests
Celero
Timer resolution: 0.001000 us
-----------------------------------------------------------------------------------------------------------------------------------------------
     Group      |   Experiment    |   Prob. Space   |     Samples     |   Iterations    |    Baseline     |  us/Iteration   | Iterations/sec  |
-----------------------------------------------------------------------------------------------------------------------------------------------
mark_twain      | grep            |               0 |               5 |               1 |         1.00000 |   1832815.00000 |            0.55 |
mark_twain      | ag              |               0 |               5 |               1 |         1.27726 |   2340976.00000 |            0.43 |
mark_twain      | ripgrep         |               0 |               5 |               1 |         0.44127 |    808766.00000 |            1.24 |
mark_twain      | fgrep_mmap      |               0 |               5 |               1 |         0.40452 |    741414.00000 |            1.35 |
mark_twain      | fgrep_stream    |               0 |               5 |               1 |         0.40558 |    743357.00000 |            1.35 |
mark_twain      | fgrep_default   |               0 |               5 |               1 |         0.40442 |    741225.00000 |            1.35 |
Complete.
```
**MacOS**

Below benchmark results are collected with 
* GNU grep 3.1
* ag 2.2.0
* rg 0.10.0
* fgrep master branch.

``` shell
./all_tests
Celero
Timer resolution: 0.001000 us
-----------------------------------------------------------------------------------------------------------------------------------------------
     Group      |   Experiment    |   Prob. Space   |     Samples     |   Iterations    |    Baseline     |  us/Iteration   | Iterations/sec  |
-----------------------------------------------------------------------------------------------------------------------------------------------
mark_twain      | grep_brew       |               0 |               5 |               1 |         1.00000 |   1290531.00000 |            0.77 |
mark_twain      | ag              |               0 |               5 |               1 |         1.69616 |   2188950.00000 |            0.46 |
mark_twain      | ripgrep         |               0 |               5 |               1 |         0.56342 |    727106.00000 |            1.38 |
mark_twain      | fgrep_mmap      |               0 |               5 |               1 |         0.48225 |    622353.00000 |            1.61 |
mark_twain      | fgrep_stream    |               0 |               5 |               1 |         0.48798 |    629750.00000 |            1.59 |
mark_twain      | fgrep_default   |               0 |               5 |               1 |         0.48698 |    628463.00000 |            1.59 |
Complete.
```

# Usage examples #

## Help message ##

``` shell
~/w/f/commands> ./fgrep -h
fgrep version 0.1.0
Hung Dang <hungptit@gmail.com>
usage:
  fgrep [<paths> ... ] options

where options are:
  -?, -h, --help                           display usage information
  -v, --verbose                            Display verbose information
  --exact-match                            Use exact matching algorithms.
  --invert-match                           Print lines that do not match
                                           given pattern.
  -i, --ignore-case                        Perform case insensitive matching.
                                           This is off by default.
  -r, -R, --recursive                      Recursively search subdirectories
                                           listed.
  --mmap                                   Use mmap to read the file content
                                           instead of read. This approach
                                           does not work well for big files.
  -c, --color                              Print out color text. This option
                                           is off by default.
  -n, --linenum                            Display line number. This option
                                           is off by default.
  -q, --quite                              Search a file until a match has
                                           been found. This option is off by
                                           default.
  -s, --stdin                              Read data from the STDIN.
  -e, -E, --pattern, --regexp <pattern>    Search pattern.
  -p, --path-regex <path_pattern>          Path regex.
```

## Search for lines from files ##

Below command will display all matched lines with file names.

```
 ~/w/f/commands> ./fgrep 'include.*matcher' fgrep.cpp
fgrep.cpp:#include "utils/matchers.hpp"
fgrep.cpp:#include "utils/regex_matchers.hpp"
```

Use **-i** to ignore the case

``` shell
 ~/w/f/commands> ./fgrep 'Include.*matcher' -i fgrep.cpp
fgrep.cpp:#include "utils/matchers.hpp"
fgrep.cpp:#include "utils/regex_matchers.hpp"
```

Use **invert-match** option to display lines that does not match specified regular expression.

``` shell
 ~/w/f/commands> ./fgrep 'include.*matcher' fgrep.cpp --invert-match
fgrep.cpp:#include "clara.hpp"
fgrep.cpp:#include "fmt/format.h"
fgrep.cpp:#include "grep.hpp"
fgrep.cpp:#include "ioutils/reader.hpp"
fgrep.cpp:#include "ioutils/regex_store_policies.hpp"
fgrep.cpp:#include "ioutils/search.hpp"
fgrep.cpp:#include "ioutils/search_params.hpp"
fgrep.cpp:#include "ioutils/simple_store_policy.hpp"
fgrep.cpp:#include "ioutils/stream.hpp"
fgrep.cpp:#include "params.hpp"
fgrep.cpp:#include <deque>
fgrep.cpp:#include <string>
fgrep.cpp:#include <vector>
fgrep.cpp:
fgrep.cpp:/**
fgrep.cpp: * The grep execution process has two steps:
fgrep.cpp: * 1. Expand the search paths and get the list of search files.
fgrep.cpp: * 2. Search for given pattern using file contents and display the search results.
fgrep.cpp: */
```

## Search for lines from files that match specified path regex ##

One fgrep feature that is not supported by any grep command is it allows users to specify the path patterns. Below command will search for lines in java source code in RocksDB repository. This feature is very useful when we want to grep through our desired files. 

``` shell
 ~/w/f/commands> ./fgrep 'zstd' ../../3p/src/rocksdb/ -p '[.]java$'
../../3p/src/rocksdb//java/src/main/java/org/rocksdb/CompressionType.java:  ZSTD_COMPRESSION((byte)0x7, "zstd"),
```

## Search for a pattern from selected set of files ##

Search for a zstd pattern from *.cc file in rocksdb repository

``` shell
ATH020224:src hdang$ time fgrep zstd rocksdb/ -p '[.](cc)$' -n
rocksdb//table/format.cc:557:        static char zstd_corrupt_msg[] =
rocksdb//table/format.cc:559:        return Status::Corruption(zstd_corrupt_msg);
rocksdb//tools/db_bench_tool.cc:694:  else if (!strcasecmp(ctype, "zstd"))
rocksdb//tools/ldb_cmd.cc:557:    } else if (comp == "zstd") {
rocksdb//tools/db_stress.cc:435:  else if (!strcasecmp(ctype, "zstd"))
rocksdb//tools/ldb_tool.cc:52:             "=<no|snappy|zlib|bzip2|lz4|lz4hc|xpress|zstd>\n");

real    0m0.046s
user    0m0.017s
sys     0m0.027s
```

Search for the usage of **kZSTD** in rocksdb codebase using case insensitive option

``` shell
ATH020224:src hdang$ time fgrep -i kzstd rocksdb/ -p '[.](cc|h)$' -n
rocksdb//db/db_test2.cc:1056:    compression_types.push_back(kZSTD);
rocksdb//db/db_statistics_test.cc:39:    type = kZSTD;
rocksdb//db/db_test.cc:892:    type = kZSTD;
rocksdb//table/block_based_table_builder.cc:166:    case kZSTD:
rocksdb//table/block_based_table_builder.cc:167:    case kZSTDNotFinalCompression:
rocksdb//table/format.cc:553:    case kZSTD:
rocksdb//table/format.cc:554:    case kZSTDNotFinalCompression:
rocksdb//table/table_test.cc:604:    compression_types.emplace_back(kZSTD, false);
rocksdb//table/table_test.cc:605:    compression_types.emplace_back(kZSTD, true);
rocksdb//utilities/column_aware_encoding_exp.cc:84:        {"kZSTD", CompressionType::kZSTD}};
rocksdb//java/rocksjni/portal.h:2173:      case rocksdb::CompressionType::kZSTD:
rocksdb//java/rocksjni/portal.h:2201:        return rocksdb::CompressionType::kZSTD;
rocksdb//include/rocksdb/options.h:66:  kZSTD = 0x7,
rocksdb//include/rocksdb/options.h:68:  // Only use kZSTDNotFinalCompression if you have to use ZSTD lib older than
rocksdb//include/rocksdb/options.h:71:  // RocksDB that doesn't have kZSTD. Otherwise, you should use kZSTD. We will
rocksdb//include/rocksdb/options.h:73:  kZSTDNotFinalCompression = 0x40,
rocksdb//util/compression.h:118:    case kZSTDNotFinalCompression:
rocksdb//util/compression.h:120:    case kZSTD:
rocksdb//util/compression.h:144:    case kZSTD:
rocksdb//util/compression.h:145:    case kZSTDNotFinalCompression:
rocksdb//tools/db_bench_tool.cc:695:    return rocksdb::kZSTD;
rocksdb//tools/db_bench_tool.cc:1915:      case rocksdb::kZSTD:
rocksdb//tools/db_bench_tool.cc:2852:      case rocksdb::kZSTD:
rocksdb//tools/ldb_cmd.cc:558:      opt.compression = kZSTD;
rocksdb//tools/db_stress.cc:436:    return rocksdb::kZSTD;
rocksdb//tools/sst_dump_tool.cc:72:        {CompressionType::kZSTD, "kZSTD"}};
rocksdb//tools/db_sanity_test.cc:189:    options_.compression = kZSTD;
rocksdb//options/options_helper.h:626:        {"kZSTD", kZSTD},
rocksdb//options/options_helper.h:627:        {"kZSTDNotFinalCompression", kZSTDNotFinalCompression},
rocksdb//options/options_test.cc:60:       "kZSTD:"
rocksdb//options/options_test.cc:61:       "kZSTDNotFinalCompression"},
rocksdb//options/options_test.cc:154:  ASSERT_EQ(new_cf_opt.compression_per_level[7], kZSTD);
rocksdb//options/options_test.cc:155:  ASSERT_EQ(new_cf_opt.compression_per_level[8], kZSTDNotFinalCompression);

real    0m0.080s
user    0m0.024s
sys     0m0.045s
```

# FAQs #

## Where can I find the binary for fgrep ##

All fgrep binary for Linux and MacOS can be found from this [github repository](https://github.com/hungptit/tools.git).

## Does fastgrep try to compete with grep and/or ripgrep? ##

**No**. I think grep or ripgrep are feature complete and it is impossible to keep up with these commands in term of usability. fastgrep written as a library so it can be used in other projects. I only implemented some core features and it takes a lot of time and effort to create something similar to grep or ripgrep.

## Can I contribute to fastgrep? ##

Any contribution is very welcome. I use [google coding standard](https://google.github.io/styleguide/cppguide.html) for all fastgrep related repositories.
