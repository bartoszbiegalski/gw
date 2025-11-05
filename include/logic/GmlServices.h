#pragma once

#include "core/types.h"
#include "core/Object.h"
#include "io/GmlImport.h"
#include "io/GmlExport.h"
#include "utils/string_operations.h"
#include "utils/tree_operations.h"
#include "services/XmlConfig.h"
#include "services/XmlParser.h"
#include "logic/GmlDivide.h"

class GmlServices
{
public:
    static void PerformDivision(const std::filesystem::path &inFile, std::vector<NamespacePrefix> &nsVec);

    static std::map<std::string, std::string> GetRootInfoMap(const Object *obj);
    static std::map<std::string, std::string> GetNamespaceNodeInfoMap(const Object *obj, const NamespacePrefix &prefix);
    static std::map<std::string, int> GetClassNames(const Object *obj, const NamespacePrefix &prefix);
    static std::vector<Entry> GetClassVector(const Object *obj, const NamespacePrefix &prefix, const std::string className);

private:
    static int getElementAmount(const Object *obj);

    static int getNamespaceElementAmount(const Object *obj, const NamespacePrefix &prefix);

    static NamespaceData getNamespaceDataFromPrefix(const Object *obj, const NamespacePrefix &prefix);

    // static std::vector<Entry> getClassVector(const Object *obj, const NamespacePrefix &prefix, const std::string className);
};