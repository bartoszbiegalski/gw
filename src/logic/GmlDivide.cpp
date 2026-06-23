#include "logic/GmlDivide.h"
#include <iostream>

void GmlDivide::Divide(const std::unique_ptr<XmlConfig> &cfg, std::unique_ptr<GmlObject> &sourceObject, const std::vector<NamespacePrefix> &prefixes, std::vector<std::unique_ptr<GmlObject>> &dividedObjects)
{
    std::string extension = cfg.get()->get("gml_extension", "");
    for (auto ns : prefixes)
    {
        if (sourceObject->hasNamespace(ns))
        {
            auto newObj = std::make_unique<GmlObject>();
            std::string fileSuffix = cfg.get()->get("xsd.xsd_extensions." + ns, "");
            if (fileSuffix == "")
            {
                fileSuffix = '-' + ns;
            }

            GmlMap gmlMap;
            gmlMap.emplace(ns, sourceObject->getGmlStorage().getGmlMap().at(ns));

            GmlStorage gmlStorage;
            gmlStorage.setGmlMap(gmlMap);
            const std::string newFileName = sourceObject->getFilePath().stem().u8string() + fileSuffix + extension;
            newObj->setFileName(newFileName);
            newObj->setFilePath((sourceObject->getFilePath().parent_path() / newFileName).u8string());
            newObj->setComment(sourceObject.get()->getComment());
            newObj->setNamespaceMap(sourceObject->getNamespaceMap());
            newObj->setGmlStorage(gmlStorage);

            dividedObjects.push_back(std::move(newObj));
        }
    }
}

void GmlDivide::DivideFromIdVector(const std::unique_ptr<XmlConfig> &cfg, std::unique_ptr<GmlObject> &sourceObject, const NamespacePrefix &prefix, const std::vector<GmlId> idVector, std::unique_ptr<GmlObject> &dividedObject)
{
    std::string extension = cfg.get()->get("gml_extension", "");
    if (sourceObject->hasNamespace(prefix))
    {
        auto newObj = std::make_unique<GmlObject>();
        std::string fileSuffix = cfg.get()->get("xsd.xsd_extensions." + prefix, "");
        if (fileSuffix == "")
        {
            fileSuffix = '-' + prefix;
        }

        GmlMap gmlMap;
        gmlMap.emplace(prefix, sourceObject->getGmlStorage().getGmlMap().at(prefix));

        GmlStorage gmlStorage;
        gmlStorage.setGmlMap(gmlMap);
        const std::string newFileName = sourceObject->getFilePath().stem().u8string() + fileSuffix + extension;
        newObj->setFileName(newFileName);
        newObj->setFilePath((sourceObject->getFilePath().parent_path() / newFileName).u8string());
        newObj->setComment(sourceObject.get()->getComment());
        newObj->setNamespaceMap(sourceObject->getNamespaceMap());
        newObj->setGmlStorage(gmlStorage);
        for (auto id : idVector)
        {
            gml_operations::CopyElementWithId(sourceObject, newObj, prefix, id);
        }
    }
}