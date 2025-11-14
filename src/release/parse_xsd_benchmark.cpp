#include <iostream>
#include "benchmark.h"

int main(int argc, char *argv[])
{
    auto importBenchmarkPath = std::filesystem::path("import_benchmark");
    auto parseXsdBenchmarkPath = std::filesystem::path();
    auto divideBenchmarkPath = std::filesystem::path();
    auto mergeBenchmarkPath = std::filesystem::path();
    auto exportBenchmarkPath = std::filesystem::path();
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filePath> \n";
        return 1;
    }
    std::filesystem::path filePath(argv[1]);
    auto cfg = std::make_unique<XmlConfig>(static_config::staticData);
    std::unique_ptr<Object> obj = std::make_unique<Object>();
    GmlImport::Import(filePath, obj);
    NamespaceTool::Process(cfg, obj);
    XmlParser::SetContent(cfg, obj);
    auto nsVector = std::vector<std::string>();
    nsVector.reserve(obj.get()->getNamespaceMap().size());
    for (auto const &pair : obj.get()->getNamespaceMap())
    {
        nsVector.push_back(pair.first);
    }

    gml_benchmark::divide_benchmark(obj, nsVector, importBenchmarkPath);
}
