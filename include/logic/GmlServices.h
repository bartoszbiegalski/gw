#pragma once

#include "core/types.h"
#include "core/GmlObject.h"
#include "io/GmlCreate.h"
#include "io/GmlImport.h"
#include "io/GmlExport.h"
#include "release/static_config.h"
#include "utils/string_operations.h"
#include "utils/tree_operations.h"
#include "services/XmlConfig.h"
#include "services/XmlParser.h"
#include "logic/GmlDivide.h"
#include "logic/GmlMerge.h"

class GmlServices
{
public:
    static void PerformDivision(const std::filesystem::path &inFile, std::vector<NamespacePrefix> &nsVec, bool isZip);
    static void PerformMerge(const FilePath &inFile, std::vector<FilePath> &filePathVec);
    static void DivideFromMap(const std::unique_ptr<GmlObject> &sourceObj, std::unique_ptr<GmlObject> &destObj, std::map<std::string, std::vector<std::string>> &classMap);
    static GmlNodePtr GetElementWithKey(const std::unique_ptr<GmlObject> &obj, const GmlId gmlId, std::list<std::string> &nearestKeys);
    static GmlNodePtr GetElementWithKeyAndNs(const std::unique_ptr<GmlObject> &obj, const GmlId gmlId, const std::string &nsPrefix, std::list<std::string> &nearestKeys);

    static std::map<std::string, std::string> GetRootInfoMap(const GmlObject *obj);
    static std::map<std::string, std::string> GetNamespaceNodeInfoMap(const GmlObject *obj, const NamespacePrefix &prefix);
    static std::map<std::string, int> GetClassNames(const GmlObject *obj, const NamespacePrefix &prefix);
    static GmlMap GetGmlMap(const GmlObject *obj, const NamespacePrefix &prefix);
    static std::map<GmlId, GmlNodePtr> GetClassMap(const GmlObject *obj, const NamespacePrefix &prefix, const std::string className);

private:
    static int getElementAmount(const GmlObject *obj);
    static int getNamespaceElementAmount(const GmlObject *obj, const NamespacePrefix &prefix);
    static NamespaceData getNamespaceDataFromPrefix(const GmlObject *obj, const NamespacePrefix &prefix);
};