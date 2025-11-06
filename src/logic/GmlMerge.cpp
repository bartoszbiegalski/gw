#include "logic/GmlMerge.h"
#include <iostream>

void GmlMerge::Merge(const std::unique_ptr<XmlConfig> &cfg, const std::vector<std::unique_ptr<Object>> &baseObjects, std::unique_ptr<Object> &mergedObject)
{
    // std::string extension = cfg.get()->get("gml_extension", "");
    // mergedObject->setFileName(string_operations::get_prefix(baseObjects[0].get()->getFileName(), ".") + "-merged" + extension);
    // mergedObject->setFilePath(string_operations::get_prefix(baseObjects[0].get()->getFilePath().string(), ".") + '-' + extension);

    for (auto &gmlBaseObject : baseObjects)
    {
        MergeOne(cfg, gmlBaseObject, mergedObject);
    }
}

void GmlMerge::MergeOne(const std::unique_ptr<XmlConfig> &cfg, const std::unique_ptr<Object> &sourceObject, std::unique_ptr<Object> &mergedObject)
{
    // std::string extension = cfg.get()->get("gml_extension", "");
    // mergedObject->setFileName(string_operations::get_prefix(sourceObject.get()->getFileName(), ".") + "-merged" + extension);
    // mergedObject->setFilePath(string_operations::get_prefix(sourceObject.get()->getFilePath().string(), ".") + '-' + extension);

    const NamespaceMap &oldMap = sourceObject->getNamespaceMap();
    for (const auto &[prefix, nsValue] : oldMap)
    {
        if (mergedObject->getNamespaceMap().find(prefix) == mergedObject->getNamespaceMap().end())
        {
            mergedObject->addToNamespaceMap(prefix, nsValue);
        }
    }

    GmlStorage &destStorage = mergedObject->getGmlStorage();
    GmlMap &destMap = destStorage.getGmlMap();

    for (const auto &[prefix, sourceInnerMap] : sourceObject->getGmlStorage().getGmlMap())
    {
        auto &destInnerMap = destMap[prefix];

        for (const auto &[id, node] : sourceInnerMap)
        {
            if (destInnerMap.find(id) == destInnerMap.end())
                destInnerMap.emplace(id, node);
        }
    }
}
