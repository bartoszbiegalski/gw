#include <iostream>
#include "io/GmlCreate.h"
#include "benchmark/benchmark.h"

int main(int argc, char *argv[])
{

    if (argc < 4)
    {
        std::cerr << "Usage: " << argv[0] << " <destPath> <benchmarkName> <sourcePath_1> <sourcePath_2> <...>\n";
        return 1;
    }
    auto mergeBenchmarkPath = std::filesystem::path("merge_benchmark_" + std::string(argv[2]) + ".csv");
    std::filesystem::path filePath(argv[1]);
    auto cfg = std::make_unique<XmlConfig>(static_config::staticGmlData);
    GmlCreate::Create(cfg, filePath.parent_path(), filePath.filename());
    auto destObj = std::make_unique<GmlObject>();
    auto baseObjects = std::vector<std::unique_ptr<GmlObject>>();
    GmlImport::Import(filePath, destObj);

    for (int i = 3; i < argc; i++)
    {
        std::cout << argv[i] << '\n';
        auto newObj = std::make_unique<GmlObject>();
        auto newObjPath = std::filesystem::path(argv[i]);

        GmlImport::Import(newObjPath, newObj);
        NamespaceTool::Process(cfg, newObj);
        XmlParser::SetContent(cfg, newObj);
        baseObjects.push_back(std::move(newObj));
    }
    gml_benchmark::merge_benchmark(baseObjects, destObj, mergeBenchmarkPath);
}
