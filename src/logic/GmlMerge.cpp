#include "logic/GmlMerge.h"
#include <iostream>

void GmlMerge::Merge(const std::unique_ptr<XmlConfig> &cfg, const std::vector<Object> &baseObjects, std::unique_ptr<Object> &mergedObject)
{
    std::string extension = cfg.get()->get("gml_extension", "");
    mergedObject->setFileName(string_operations::get_prefix(baseObjects[0].getFileName(), ".") + "-merged" + extension);
    mergedObject->setFilePath(string_operations::get_prefix(baseObjects[0].getFilePath().string(), ".") + '-' + extension);

    for (auto &gmlBaseObject : baseObjects)
    {
        for (auto ns : gmlBaseObject.getNamespaceMap())
        {
            const NamespaceMap oldMap = gmlBaseObject.getNamespaceMap();
            for (auto elem : oldMap)
            {
                if (mergedObject->getNamespaceMap().find(elem.first) == mergedObject->getNamespaceMap().end())
                {
                    mergedObject->addToNamespaceMap(elem.first, elem.second);
                }
                // mergedObject.setNamespaceMap(mergedObject.getNamespaceMap()[elem.first]);
            }
        }

        GmlStorage &destStorage = mergedObject->getGmlStorage(); // <- zwraca GmlStorage& i jest nieconst
        GmlMap &destMap = destStorage.getGmlMap();               // <- zwraca nieconst GmlMap&

        for (const auto &[prefix, sourceVec] : gmlBaseObject.getGmlStorage().getGmlMap())
        {
            std::vector<Entry> &destVec = destMap[prefix];
            destVec.insert(destVec.end(), sourceVec.begin(), sourceVec.end());
        }
    }
}