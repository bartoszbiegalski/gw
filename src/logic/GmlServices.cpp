#include "logic/GmlServices.h"
#include "services/ZipSaver.h"
#include <iostream>

void GmlServices::PerformDivision(const std::filesystem::path &inFile, std::vector<NamespacePrefix> &nsVec, bool isZip)
{
    auto cfg = std::make_unique<XmlConfig>(static_config::staticGmlData);
    std::unique_ptr<GmlObject> obj = std::make_unique<GmlObject>();
    GmlImport::Import(inFile, obj);
    NamespaceTool::Process(cfg, obj);
    XmlParser::SetContent(cfg, obj);
    auto objVec = std::vector<std::unique_ptr<GmlObject>>();

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
        std::cout << o.get()->getFileName() << '\n';
        GmlExport::Export(cfg, o);
    }

    if (isZip)
    {
        std::vector<std::string> zipFileVec;
        auto fillZipVec = [&zipFileVec](const auto &objects)
        {
            for (const auto &obj : objects)
            {
                zipFileVec.push_back(obj->getFileName());
            }
        };
        fillZipVec(objVec);
        ZipSaver::SaveToZip(obj.get()->getFilePath().stem().u8string(), obj.get()->getFilePath().parent_path(), zipFileVec);

        for (const auto &obj : objVec)
        {
            std::filesystem::remove(obj->getFilePath());
        }
    }
}

void GmlServices::PerformMerge(const FilePath &inFile, std::vector<FilePath> &filePathVec)
{
    auto cfg = std::make_unique<XmlConfig>(static_config::staticGmlData);

    GmlCreate::Create(cfg, inFile.parent_path(), inFile.filename().string());
    std::unique_ptr<GmlObject> destObj = std::make_unique<GmlObject>();
    GmlImport::Import(inFile, destObj);

    for (auto sourceFilePath : filePathVec)
    {
        auto newObj = std::make_unique<GmlObject>();

        GmlImport::Import(sourceFilePath, newObj);
        NamespaceTool::Process(cfg, newObj);
        XmlParser::SetContent(cfg, newObj);

        GmlMerge::MergeOne(cfg, newObj, destObj);
    }

    GmlExport::Export(cfg, destObj);
}

void GmlServices::DivideFromMap(const std::unique_ptr<GmlObject> &sourceObj, std::unique_ptr<GmlObject> &destObj, std::map<std::string, std::vector<std::string>> &classMap)
{
    const NamespaceMap &oldMap = sourceObj->getNamespaceMap();
    for (const auto &[prefix, nsValue] : oldMap)
    {
        if (destObj->getNamespaceMap().find(prefix) ==
            destObj->getNamespaceMap().end())
        {
            destObj->addToNamespaceMap(prefix, nsValue);
        }
    }

    auto &srcMap = sourceObj->getGmlStorage().getGmlMap();
    auto &destMap = destObj->getGmlStorage().getGmlMap();

    for (const auto &[prefix, classNameVec] : classMap)
    {
        auto srcIt = srcMap.find(prefix);
        if (srcIt == srcMap.end())
            continue;

        auto classNameCountMap =
            GmlServices::GetClassNames(sourceObj.get(), prefix);

        for (const auto &className : classNameVec)
        {
            if (classNameCountMap.find(className) == classNameCountMap.end())
                continue;

            auto classMapForPrefix =
                GmlServices::GetClassMap(sourceObj.get(), prefix, className);

            destMap[prefix].insert(
                classMapForPrefix.begin(),
                classMapForPrefix.end());
        }
    }
}

GmlNodePtr GmlServices::GetElementWithKey(const std::unique_ptr<GmlObject> &obj, const GmlId gmlId, std::list<std::string> &nearestKeys)
{
    std::map<std::string, int> nearestKeysMap{};
    for (auto nsMap : obj.get()->getGmlStorage().getGmlMap())
    {
        if (nsMap.second.find(gmlId) != nsMap.second.end())
        {
            return nsMap.second.at(gmlId);
        }
        else
        {
            for (auto id : nsMap.second)
            {
                if (nearestKeysMap.empty() == true)
                {
                    nearestKeysMap.emplace(id.first, string_operations::levenstein_distance(id.first, gmlId));
                }
                else if (nearestKeysMap.begin()->second > string_operations::levenstein_distance(id.first, gmlId)) // new nearest Id
                {
                    nearestKeysMap.clear();
                    nearestKeysMap.emplace(id.first, string_operations::levenstein_distance(id.first, gmlId));
                }
                else if (nearestKeysMap.begin()->second >= string_operations::levenstein_distance(id.first, gmlId)) // push back
                {
                    nearestKeysMap.emplace(id.first, string_operations::levenstein_distance(id.first, gmlId));
                }
            }
        }
    }
    for (const auto &[key, dist] : nearestKeysMap)
    {
        nearestKeys.push_back(key);
    }
    return nullptr;
}

std::map<std::string, std::string> GmlServices::GetRootInfoMap(const GmlObject *obj)
{
    auto rootInfoMap = std::map<std::string, std::string>();
    std::string valityString = obj->getXmlValidity() == true ? "tak " : "nie";

    rootInfoMap.insert(std::make_pair("Elementy:", std::to_string(getElementAmount(obj))));
    rootInfoMap.insert(std::make_pair("Czy poprawny XML: ", valityString));

    return rootInfoMap;
}

std::map<std::string, std::string> GmlServices::GetNamespaceNodeInfoMap(const GmlObject *obj, const NamespacePrefix &prefix)
{
    auto namespaceNodeMap = std::map<std::string, std::string>();
    std::string valityString = obj->getGmlStorage().getXsdValidity(prefix) == true ? "tak " : "nie";

    namespaceNodeMap.insert(std::make_pair("Elementy: ", std::to_string(getNamespaceElementAmount(obj, prefix))));
    namespaceNodeMap.insert(std::make_pair("Plik XSD: ", getNamespaceDataFromPrefix(obj, prefix).NamespaceUrl));
    namespaceNodeMap.insert(std::make_pair("Czy poprawny XSD: ", valityString));

    return namespaceNodeMap;
}

int GmlServices::getElementAmount(const GmlObject *obj)
{
    return std::accumulate(obj->getGmlStorage().getGmlMap().begin(),
                           obj->getGmlStorage().getGmlMap().end(), 0,
                           [&](int acc, const auto &pair)
                           {
                               return acc + pair.second.size();
                           });
}

int GmlServices::getNamespaceElementAmount(const GmlObject *obj, const NamespacePrefix &prefix)
{
    if (obj->getGmlStorage().getGmlMap().find(prefix) == obj->getGmlStorage().getGmlMap().end())
        return 0;
    else
        return obj->getGmlStorage().getGmlMap().at(prefix).size();
}

NamespaceData GmlServices::getNamespaceDataFromPrefix(const GmlObject *obj, const NamespacePrefix &prefix)
{
    if (obj->getNamespaceMap().find(prefix) == obj->getNamespaceMap().end())
        return NamespaceData{};
    else
        return obj->getNamespaceMap().at(prefix);
}

GmlMap GmlServices::GetGmlMap(const GmlObject *obj, const NamespacePrefix &prefix)
{
    const auto &gmlMap = obj->getGmlStorage().getGmlMap();
    return gmlMap;
}

std::vector<std::string> GmlServices::getReferenceInfo(const std::string &prefix, const std::string &className)
{
    std::vector<std::string> referenceElements;
    return referenceElements;
}

std::map<std::string, int> GmlServices::GetClassNames(const GmlObject *obj, const NamespacePrefix &prefix)
{
    std::map<std::string, int> classNameMap{};

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

std::map<GmlId, GmlNodePtr> GmlServices::GetClassMap(const GmlObject *obj, const NamespacePrefix &prefix, const std::string className)
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

std::vector<GmlId> GmlServices::GetReferences(const std::unique_ptr<GmlObject> &sourceObject, const std::map<std::string, std::vector<std::string>> &classMap)
{
    auto xmlCfg = std::make_unique<XmlConfig>(static_config::staticGmlData);
    std::string referenceAttr = xmlCfg.get()->get("reference_attr", "href");
    auto idVector = std::vector<GmlId>();
    for (auto &[prefix, classVector] : classMap)
    {
        std::string referenceFile = xmlCfg.get()->get("xsd.reference_files." + prefix, "");
        auto refConfig = std::make_unique<XsdConfig>(FilePath{"resources"} / referenceFile);
        for (auto &className : classVector)
        {
            auto refVec = refConfig.get()->get_json(className);
            std::vector<std::string> refs;
            for (const auto &obj : refVec)
            {
                if (obj.contains("referencesTo"))
                {
                    refs = obj["referencesTo"].get<std::vector<std::string>>();
                }
            }
            auto gmlMap = GmlServices::GetClassMap(sourceObject.get(), prefix, className);

            for (auto &[id, ptr] : gmlMap)
            {
                for (auto referenceElement : refs)
                {
                    tree_operations::traverse_gml_id(ptr.get(), referenceElement, referenceAttr, idVector, prefix);
                }
            }
        }
    }
    return idVector;
}
