#pragma once

/**
 * @brief Header with operations on GmlObject class
 */

#include "core/types.h"
#include "core/GmlObject.h"
#include "utils/string_operations.h"

namespace gml_operations
{
    std::map<std::string, std::string> GetRootInfoMap(const std::unique_ptr<GmlObject> &obj);
    std::map<std::string, std::string> GetNamespaceNodeInfoMap(const std::unique_ptr<GmlObject> &obj, const NamespacePrefix &prefix);

    void DivideFromMap(const std::unique_ptr<GmlObject> &sourceObj, std::unique_ptr<GmlObject> &destObj, std::map<NamespacePrefix, std::vector<std::string>> &classMap);
    void CopyElementWithId(const std::unique_ptr<GmlObject> &sourceObj, std::unique_ptr<GmlObject> &destObj, NamespacePrefix prefix, GmlId id);

    NamespaceData GetNamespaceDataFromPrefix(const std::unique_ptr<GmlObject> &obj, const NamespacePrefix &prefix);
    std::map<GmlId, GmlNodePtr> GetClassMap(const std::unique_ptr<GmlObject> &obj, const NamespacePrefix &prefix, const std::string className);
    GmlNodePtr GetElementWithKey(const std::unique_ptr<GmlObject> &obj, const GmlId gmlId, std::list<std::string> &nearestKeys);
}
