#include <iostream>
#include "benchmark/benchmark.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <filePath> <benchmark_name> \n";
        return 1;
    }
    std::filesystem::path filePath(argv[1]);
    auto importBenchmarkPath = std::filesystem::path("import_benchmark_" + std::string(argv[2]) + ".csv");

    gml_benchmark::import_benchmark(filePath, importBenchmarkPath);
}
