#include <iostream>
#include "benchmark/benchmark.h"

int main(int argc, char *argv[])
{
    auto exportBenchmarkPath = std::filesystem::path("export_benchmark");
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

    gml_benchmark::export_benchmark(obj, exportBenchmarkPath);
}
