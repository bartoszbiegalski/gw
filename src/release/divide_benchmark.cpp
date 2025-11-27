#include <iostream>
#include "benchmark/benchmark.h"

int main(int argc, char *argv[])
{
    auto divideBenchmarkPath = std::filesystem::path("divide_benchmark.csv");
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filePath> \n";
        return 1;
    }
    std::filesystem::path filePath(argv[1]);
    auto cfg = std::make_unique<XmlConfig>(static_config::staticData);
    std::unique_ptr<GmlObject> obj = std::make_unique<GmlObject>();
    GmlImport::Import(filePath, obj);
    NamespaceTool::Process(cfg, obj);
    XmlParser::SetContent(cfg, obj);
    auto nsVector = std::vector<std::string>();
    nsVector.reserve(obj.get()->getGmlStorage().getGmlMap().size());
    for (auto const &pair : obj.get()->getGmlStorage().getGmlMap())
    {
        nsVector.push_back(pair.first);
    }

    gml_benchmark::divide_benchmark(obj, nsVector, divideBenchmarkPath);
    std::cout << "WORKING DIR: " << std::filesystem::current_path() << "\n";
    std::cout << "Saving to: " << divideBenchmarkPath << std::endl;
}
