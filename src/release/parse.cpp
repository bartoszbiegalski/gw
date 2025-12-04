#include <iostream>

#include <core/GmlObject.h>
#include <io/GmlImport.h>
#include <io/GmlExport.h>

#include <services/XmlConfig.h>
#include <utils/NamespaceTool.h>
#include <utils/tree_operations.h>

#include "services/XmlParser.h"
#include "services/XsdParser.h"

#include "logic/GmlDivide.h"
#include "logic/GmlMerge.h"
#include "logic/GmlServices.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filePath1>\n";
        return 1;
    }
    std::filesystem::path filePath(argv[1]);

    auto cfg = std::make_unique<XmlConfig>(static_config::staticData);
    std::unique_ptr<GmlObject> obj = std::make_unique<GmlObject>();

    GmlImport::Import(filePath, obj);
    NamespaceTool::Process(cfg, obj);
    XmlParser::SetContent(cfg, obj);
    XsdParser::Parse(cfg, obj);
    for (auto &i : obj.get()->getGmlStorage().getGmlMap())
    {
        std::cout << i.first << " " << i.second.size() << "\n";
    }

    return 0;
}
