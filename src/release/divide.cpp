// divide.exe
// Program made for C-GEO purposes, creates exe with
// <file_name> <ns_1> <ns_2> ..

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
    if (argc < 2)
    {
        return -1;
    }
    std::filesystem::path filePath(argv[1]);
    std::vector<std::string> nsVect{};

    for (int i = 2; i < argc; i++)
    {
        nsVect.push_back(std::string(argv[2]));
    }
    GmlServices::PerformDivision(filePath.string(), nsVect);

    return 0;
    // if (argc < 3)
    // {
    //     std::cerr << "Usage: " << argv[0] << " <filePath> <jsonPath>\n";
    //     return 1;
    // }
    // std::filesystem::path filePath(argv[1]);
    // std::filesystem::path jsonPath(argv[2]);

    // auto cfg = std::make_unique<XmlConfig>(jsonPath);
    // std::unique_ptr<Object> obj = std::make_unique<Object>();

    // GmlImport::Import(filePath, obj);
    // NamespaceTool::Process(cfg, obj);
    // XmlParser::SetContent(cfg, obj);
    // XsdParser::Parse(cfg, obj);
    // // for (auto &i : obj.get()->getNamespaceMap())
    // // {
    // //     std::cout << i.first << '\n';
    // // }
    // for (auto &[first, second] : GmlServices::GetRootInfoMap(obj.get()))
    // {
    //     std::cout << first << " " << second << "\n";
    // }

    // // for (auto &setElem : GmlServices::GetClassNames(obj.get(), "ot"))
    // // {
    // //     std::cout << setElem.first << " " << setElem.second << "\n";
    // // }

    // // for (auto &elem : obj.get()->getGmlStorage().getGmlMap())
    // // {
    // //     std::cout << elem.first << " " << elem.second.size() << "\n";
    // //     for (auto &[f, s] : GmlServices::GetNamespaceNodeInfoMap(obj.get(), elem.first))
    // //     {
    // //         std::cout << f << " " << s << "\n";
    // //     }
    // // }
    // // std::cout << GmlServices::GetNamespaceElementAmount(obj, "ot") << "\n";

    // // GmlExtract::Extract(obj);
    // // std::vector<NamespacePrefix> ns = {"egb", "ot"};
    // // auto objVec = std::vector<std::unique_ptr<Object>>();
    // // GmlDivide::Divide(cfg, obj, ns, objVec);

    // // GmlExport::Export(cfg, objVec[0]);
    // // GmlExport::Export(cfg, objVec[1]);

    // // for (const auto &it : objVec)
    // // {
    // //     std::cout << it.get()->getFileName() << "\n";
    // //     GmlExport::Export(cfg, it);
    // // }

    // //  std::vector<NamespacePrefix> ns = {"egb", "ot"};

    // // std::vector<Object> mergedObjects;
    // // mergedObjects.push_back(*obj);

    // // std::unique_ptr<Object> newObj = std::make_unique<Object>();

    // // GmlMerge::Merge(cfg, mergedObjects, newObj);

    // // for (auto &jt : newObj.get()->getGmlStorage().getGmlMap())
    // // {
    // //     std::cout << jt.first << " " << jt.second.size() << "\n";
    // // }

    // // NamespaceTool::UpdateIds(cfg, obj);
    // //  XsdParser::Parse(cfg, obj);

    // return 0;
}
