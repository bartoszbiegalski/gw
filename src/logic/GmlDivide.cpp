#include "logic/GmlDivide.h"
#include <iostream>

void GmlDivide::DivideFromPrefixes(const std::unique_ptr<XmlConfig> &cfg, std::unique_ptr<GmlObject> &sourceObject, const std::vector<NamespacePrefix> &prefixes, std::vector<std::unique_ptr<GmlObject>> &dividedObjects)
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
            const std::string newFileName = sourceObject->getFilePath().stem().string() + fileSuffix + extension;
            newObj->setFileName(newFileName);
            newObj->setFilePath((sourceObject->getFilePath().parent_path() / newFileName).u8string());
            newObj->setComment(sourceObject.get()->getComment());
            newObj->setNamespaceMap(sourceObject->getNamespaceMap());
            newObj->setGmlStorage(gmlStorage);

            dividedObjects.push_back(std::move(newObj));
        }
    }
}

void GmlDivide::DivideJO(const std::unique_ptr<XmlConfig> &cfg, const GmlTreeModel &model, std::unique_ptr<GmlObject> &sourceObject, std::vector<std::unique_ptr<GmlObject>> &dividedObjects)
{
    std::string extension = cfg.get()->get("gml_extension", "");
    for (auto &[teryt, m] : model)
    {
        auto newObj = std::make_unique<GmlObject>();
        std::string fileName = teryt;

        GmlMap gmlMap;
        for (auto &[className, vec] : m)
        {
            for (auto &[node, id] : vec)
            {
                gmlMap["egb"].emplace(id, std::make_shared<xmlNode>(*node));
            }
        }

        GmlStorage gmlStorage;
        gmlStorage.setGmlMap(gmlMap);
        const std::string newFileName = fileName + extension;
        newObj->setFileName(newFileName);
        newObj->setFilePath((sourceObject->getFilePath().parent_path() / newFileName).u8string());
        newObj->setComment(sourceObject.get()->getComment());
        newObj->setNamespaceMap(sourceObject->getNamespaceMap());
        newObj->setGmlStorage(gmlStorage);

        dividedObjects.push_back(std::move(newObj));
    }
}

void GmlDivide::DivideFromIdVector(const std::unique_ptr<XmlConfig> &cfg, std::unique_ptr<GmlObject> &sourceObject, const std::string &fileSuffix, const std::vector<GmlId> idVector, std::unique_ptr<GmlObject> &dividedObject)
{
    std::string extension = cfg.get()->get("gml_extension", "");
    GmlMap gmlMap;
    GmlStorage gmlStorage;
    gmlStorage.setGmlMap(gmlMap);
    const std::string newFileName = sourceObject->getFilePath().stem().string() + fileSuffix + extension;
    dividedObject->setFileName(newFileName);
    dividedObject->setFilePath((sourceObject->getFilePath().parent_path() / newFileName).u8string());
    dividedObject->setComment(sourceObject.get()->getComment());
    dividedObject->setNamespaceMap(sourceObject->getNamespaceMap());
    dividedObject->setGmlStorage(gmlStorage);

    for (auto &[prefix, _] : sourceObject.get()->getGmlStorage().getGmlMap())
    {
        for (auto id : idVector)
        {
            gml_operations::CopyElementWithId(sourceObject, dividedObject, prefix, id);
        }
    }
}