#include "logic/GmlServices.h"
#include "services/ZipSaver.h"
#include <iostream>

void GmlServices::PerformDivision(const std::filesystem::path &inFile, std::vector<NamespacePrefix> &nsVec)
{
    auto cfg = std::make_unique<XmlConfig>(static_config::staticData);
    std::unique_ptr<Object> obj = std::make_unique<Object>();
    GmlImport::Import(inFile, obj);
    NamespaceTool::Process(cfg, obj);
    XmlParser::SetContent(cfg, obj);
    auto objVec = std::vector<std::unique_ptr<Object>>();

    if (nsVec.size() < 1)
    {
        for (auto i : obj.get()->getGmlStorage().getGmlMap())
        {
            nsVec.push_back(i.first);
        }
    }

    GmlDivide::Divide(cfg, obj, nsVec, objVec);
    for (auto &o : objVec)
    {
        GmlExport::Export(cfg, o);
    }

    // if (isZip)
    // {
    //     std::vector<std::string> zipFileVec;
    //     auto fillZipVec = [&zipFileVec](const auto &objects)
    //     {
    //         for (const auto &obj : objects)
    //         {
    //             zipFileVec.push_back(obj->getFileName());
    //         }
    //     };
    //     fillZipVec(objVec);
    //     ZipSaver::SaveToZip(obj.get()->getFilePath().stem().u8string(), obj.get()->getFilePath().parent_path(), zipFileVec);

    //     for (const auto &obj : objVec)
    //     {
    //         std::filesystem::remove(obj->getFilePath());
    //     }
    // }
}

std::map<std::string, std::string> GmlServices::GetRootInfoMap(const Object *obj)
{
    auto rootInfoMap = std::map<std::string, std::string>();
    rootInfoMap.insert(std::make_pair("Elementy:", std::to_string(getElementAmount(obj))));
    std::string valityString = obj->getXmlValidity() == true ? "tak " : "nie";
    rootInfoMap.insert(std::make_pair("Czy poprawny XML: ", valityString));

    // returnMap.insert(std::make_pair("Czy poprawny pod wzgledem xml: ", TODO: tu jakas funkcja)); cos chcemy z ta walidacja

    return rootInfoMap;
}

std::map<std::string, std::string> GmlServices::GetNamespaceNodeInfoMap(const Object *obj, const NamespacePrefix &prefix)
{
    auto namespaceNodeMap = std::map<std::string, std::string>();
    namespaceNodeMap.insert(std::make_pair("Elementy: ", std::to_string(getNamespaceElementAmount(obj, prefix))));
    namespaceNodeMap.insert(std::make_pair("Plik XSD: ", getNamespaceDataFromPrefix(obj, prefix).NamespaceUrl));
    std::string valityString = obj->getGmlStorage().getXsdValidity(prefix) == true ? "tak " : "nie";
    namespaceNodeMap.insert(std::make_pair("Czy poprawny XSD: ", valityString));

    return namespaceNodeMap;
}

int GmlServices::getElementAmount(const Object *obj)
{
    return std::accumulate(obj->getGmlStorage().getGmlMap().begin(),
                           obj->getGmlStorage().getGmlMap().end(), 0,
                           [&](int acc, const auto &pair)
                           {
                               return acc + pair.second.size();
                           });
}

int GmlServices::getNamespaceElementAmount(const Object *obj, const NamespacePrefix &prefix)
{
    if (obj->getGmlStorage().getGmlMap().find(prefix) == obj->getGmlStorage().getGmlMap().end())
        return 0;
    else
        return obj->getGmlStorage().getGmlMap().at(prefix).size();
}

NamespaceData GmlServices::getNamespaceDataFromPrefix(const Object *obj, const NamespacePrefix &prefix)
{
    if (obj->getNamespaceMap().find(prefix) == obj->getNamespaceMap().end())
        return NamespaceData{};
    else
        return obj->getNamespaceMap().at(prefix);
}

std::map<std::string, int> GmlServices::GetClassNames(const Object *obj, const NamespacePrefix &prefix)
{
    std::map<std::string, int> classNameMap;

    const auto &gmlMap = obj->getGmlStorage().getGmlMap();
    auto found = gmlMap.find(prefix);
    if (found == gmlMap.end())
        return classNameMap;

    const auto &entries = found->second;

    for (const auto &[nsPrefix, vector] : entries)
    {
        for (auto it = vector.get(); it != nullptr; it = it->next)
        {
            if (it->type == XML_TEXT_NODE || it->name == nullptr)
                continue;

            std::string nodeName(reinterpret_cast<const char *>(it->name));

            classNameMap[nodeName]++;
        }
    }

    return classNameMap;
}

std::map<GmlId, GmlNodePtr> GmlServices::GetClassMap(const Object *obj, const NamespacePrefix &prefix, const std::string className)
{
    auto entryMap = std::map<GmlId, GmlNodePtr>();
    if (obj->getGmlStorage().getGmlMap().find(prefix) == obj->getGmlStorage().getGmlMap().end())
    {
        return entryMap;
    }
    else
    {
        for (auto &[prefix, gmlSharedPtr] : obj->getGmlStorage().getGmlMap().at(prefix))
        {
            auto it = gmlSharedPtr.get();
            while (it != nullptr && it->type != XML_TEXT_NODE)
            {
                if (reinterpret_cast<const char *>(it->name) == className)
                {
                    entryMap.emplace(prefix, gmlSharedPtr);
                }
                it = it->next;
            }
        }
    }
    return entryMap;
}
