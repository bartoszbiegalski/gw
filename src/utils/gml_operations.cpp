/**
 * @file gml_operations.cpp
 * @brief Definitions of operations on GmlObjects
 */

#include "utils/gml_operations.h"

namespace gml_operations
{
    std::map<std::string, std::string> GetRootInfoMap(const std::unique_ptr<GmlObject> &obj)
    {
        auto rootInfoMap = std::map<std::string, std::string>();
        std::string valityString = obj.get()->getXmlValidity() == true ? "tak " : "nie";

        rootInfoMap.insert(std::make_pair("Elementy:", std::to_string(obj.get()->getElementAmount())));
        rootInfoMap.insert(std::make_pair("Czy poprawny XML: ", valityString));

        return rootInfoMap;
    }

    std::map<std::string, std::string> GetNamespaceNodeInfoMap(const std::unique_ptr<GmlObject> &obj, const NamespacePrefix &prefix)
    {
        auto namespaceNodeMap = std::map<std::string, std::string>();
        std::string valityString = obj.get()->getGmlStorage().getXsdValidity(prefix) == true ? "tak " : "nie";

        namespaceNodeMap.insert(std::make_pair("Elementy: ", std::to_string(obj.get()->getNamespaceElementAmount(prefix))));
        namespaceNodeMap.insert(std::make_pair("Plik XSD: ", GetNamespaceDataFromPrefix(obj, prefix).NamespaceUrl));
        namespaceNodeMap.insert(std::make_pair("Czy poprawny XSD: ", valityString));

        return namespaceNodeMap;
    }

    NamespaceData GetNamespaceDataFromPrefix(const std::unique_ptr<GmlObject> &obj, const NamespacePrefix &prefix)
    {
        if (obj.get()->getNamespaceMap().find(prefix) == obj.get()->getNamespaceMap().end())
            return NamespaceData{};
        else
            return obj.get()->getNamespaceMap().at(prefix);
    }

    std::map<GmlId, GmlNodePtr> GetClassMap(const std::unique_ptr<GmlObject> &obj, const NamespacePrefix &prefix, const std::string className)
    {
        auto entryMap = std::map<GmlId, GmlNodePtr>();
        if (obj.get()->getGmlStorage().getGmlMap().find(prefix) == obj.get()->getGmlStorage().getGmlMap().end())
        {
            return entryMap;
        }
        else
        {
            for (auto &[prefix, gmlSharedPtr] : obj.get()->getGmlStorage().getGmlMap().at(prefix))
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

    GmlNodePtr GetElementWithKey(const std::unique_ptr<GmlObject> &obj, const GmlId gmlId, std::list<std::string> &nearestKeys)
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
    void DivideFromMap(const std::unique_ptr<GmlObject> &sourceObj, std::unique_ptr<GmlObject> &destObj, std::map<NamespacePrefix, std::vector<std::string>> &classMap)
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

            auto classNameCountMap = sourceObj.get()->GetClassNames(prefix);

            for (const auto &className : classNameVec)
            {
                if (classNameCountMap.find(className) == classNameCountMap.end())
                    continue;

                auto classMapForPrefix = gml_operations::GetClassMap(sourceObj, prefix, className);

                destMap[prefix].insert(
                    classMapForPrefix.begin(),
                    classMapForPrefix.end());
            }
        }
    }
    void DivideFromClassVector(const std::unique_ptr<GmlObject> &sourceObj, std::unique_ptr<GmlObject> &destObj, std::vector<ClassName> &classVec)
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

        for (const auto &className : classVec)
        {
            for (auto &[prefix, idMap] : srcMap)
            {
                auto classNameCountMap = sourceObj.get()->GetClassNames(prefix);
                if (classNameCountMap.find(className) == classNameCountMap.end())
                    continue;

                auto classMapForPrefix = gml_operations::GetClassMap(sourceObj, prefix, className);

                destMap[prefix].insert(
                    classMapForPrefix.begin(),
                    classMapForPrefix.end());
            }
        }
    }

    void DivideFromIdVector(const std::unique_ptr<GmlObject> &sourceObj, std::unique_ptr<GmlObject> &destObj, std::vector<ClassName> &idVector)
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

        for (const auto &id : idVector)
        {
            for (auto &[prefix, idMap] : srcMap)
            {
                if (idMap.find(id) != idMap.end())
                {
                    destMap[prefix].emplace(std::make_pair(id, idMap[id]));
                }
            }
        }
    }

    void CopyElementWithId(const std::unique_ptr<GmlObject> &sourceObj, std::unique_ptr<GmlObject> &destObj, NamespacePrefix prefix, GmlId id)
    {
        if (sourceObj.get()->getGmlStorage().getGmlMap()[prefix].find(id) != sourceObj.get()->getGmlStorage().getGmlMap()[prefix].end())
        {
            auto node = sourceObj.get()->getGmlStorage().getGmlMap()[prefix].at(id);
            destObj.get()->getGmlStorage().getGmlMap()[prefix].emplace(id, node);
        }
    }
}