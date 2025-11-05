#include "logic/GmlDivide.h"
#include <iostream>

void GmlDivide::Divide(const std::unique_ptr<XmlConfig> &cfg, std::unique_ptr<Object> &obj, const std::vector<NamespacePrefix> vNamespaces, std::vector<std::unique_ptr<Object>> &dividedObjects)
{
    std::string extension = cfg.get()->get("gml_extension", "");
    for (auto ns : vNamespaces)
    {
        if (obj->hasNamespace(ns))
        {
            auto newObj = std::make_unique<Object>();
            std::string fileSuffix = cfg.get()->get("xsd.xsd_extensions." + ns, "");
            if (fileSuffix == "")
            {
                fileSuffix = '-' + ns;
            }

            GmlMap gmlMap;
            gmlMap.emplace(ns, obj->getGmlStorage().getGmlMap().at(ns));

            GmlStorage gmlStorage;
            gmlStorage.setGmlMap(gmlMap);
            const std::string newFileName = obj->getFilePath().stem().u8string() + fileSuffix + extension;
            newObj->setFileName(newFileName);
            newObj->setFilePath((obj->getFilePath().parent_path() / newFileName).u8string());
            newObj->setNamespaceMap(obj->getNamespaceMap());
            newObj->setGmlStorage(gmlStorage);

            // nadajemy nowe id od startu
            //  newObj->setGmlStorage();

            dividedObjects.push_back(std::move(newObj));
        }
    }
}