#include <iostream>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include <core/Object.h>
#include <io/GmlCreate.h>
#include <io/GmlImport.h>
#include <io/GmlExport.h>
#include "logic/GmlMerge.h"
#include "logic/GmlServices.h"
#include "release/static_config.h"
#include <services/XmlConfig.h>
#include "services/XmlParser.h"
#include "services/XsdParser.h"
#include <utils/NamespaceTool.h>
#include <utils/tree_operations.h>

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <destPath> <sourcePath_1> <sourcePath_2> <...>\n";
        return 1;
    }
    auto path = std::filesystem::path(argv[1]);
    auto cfg = std::make_unique<XmlConfig>(static_config::staticData);

    GmlCreate::Create(cfg, path.parent_path(), path.filename());
    std::unique_ptr<Object> destObj = std::make_unique<Object>();
    GmlImport::Import(path, destObj);

    if (argc < 3)
    {
        std::cerr << "Provided no files\n";
        return 1;
    }

    // std::unique_ptr<Object> destObj = std::make_unique<Object>();

    for (int i = 2; i < argc; i++)
    {
        std::cout << argv[i] << '\n';
        auto newObj = std::make_unique<Object>();
        auto newObjPath = std::filesystem::path(argv[i]);

        GmlImport::Import(newObjPath, newObj);
        NamespaceTool::Process(cfg, newObj);
        XmlParser::SetContent(cfg, newObj);

        GmlMerge::MergeOne(cfg, newObj, destObj);
    }

    GmlExport::Export(cfg, destObj);

    // GmlImport::Import(filePath, obj);
    // NamespaceTool::Process(cfg, obj);
    // XmlParser::SetContent(cfg, obj);
    // for (auto &i : obj.get()->getGmlStorage().getGmlMap())
    // {
    //     std::cout << i.first << " " << i.second.size() << "\n";
    // }
    // return 0;
}
