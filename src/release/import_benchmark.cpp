#include <iostream>
#include "benchmark.h"

int main(int argc, char *argv[])
{
    auto importBenchmarkPath = std::filesystem::path("import_benchmark.csv");
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filePath> \n";
        return 1;
    }
    std::filesystem::path filePath(argv[1]);

    gml_benchmark::import_benchmark(filePath, importBenchmarkPath);
}
