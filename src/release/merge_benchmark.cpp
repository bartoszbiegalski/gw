#include <iostream>
#include "io/GmlCreate.h"
#include "benchmark.h"

int main(int argc, char *argv[])
{

    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <destPath> <sourcePath_1> <sourcePath_2> <...>\n";
        return 1;
    }
    auto mergeBenchmarkPath = std::filesystem::path("merge_benchmark.csv");
    std::filesystem::path filePath(argv[1]);
    auto cfg = std::make_unique<XmlConfig>(static_config::staticData);
    GmlCreate::Create(cfg, filePath.parent_path(), filePath.filename());
    auto destObj = std::make_unique<Object>();
    auto baseObjects = std::vector<std::unique_ptr<Object>>();
    GmlImport::Import(filePath, destObj);

    for (int i = 2; i < argc; i++)
    {
        std::cout << argv[i] << '\n';
        auto newObj = std::make_unique<Object>();
        auto newObjPath = std::filesystem::path(argv[i]);

        GmlImport::Import(newObjPath, newObj);
        NamespaceTool::Process(cfg, newObj);
        XmlParser::SetContent(cfg, newObj);
        baseObjects.push_back(std::move(newObj));
    }
    gml_benchmark::merge_benchmark(baseObjects, destObj, mergeBenchmarkPath);
}
