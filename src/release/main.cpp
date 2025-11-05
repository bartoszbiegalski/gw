#include <iostream>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include <core/Object.h>
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
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <filePath> <jsonPath>\n";
        return 1;
    }
    std::filesystem::path filePath(argv[1]);
    std::filesystem::path jsonPath(argv[2]);

    auto cfg = std::make_unique<XmlConfig>(jsonPath);
    std::unique_ptr<Object> obj = std::make_unique<Object>();

    GmlImport::Import(filePath, obj);
    NamespaceTool::Process(cfg, obj);
    XmlParser::SetContent(cfg, obj);
    return 0;
}
