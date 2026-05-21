#pragma once

#include "core/types.h"
#include "core/GmlObject.h"
#include "io/GmlCreate.h"
#include "io/GmlImport.h"
#include "io/GmlExport.h"
#include "release/static_config.h"
#include "utils/string_operations.h"
#include "utils/tree_operations.h"
#include "utils/gml_operations.h"
#include "services/XmlConfig.h"
#include "services/XsdConfig.h"
#include "services/XmlParser.h"
#include "logic/GmlDivide.h"
#include "logic/GmlMerge.h"

class GmlServices
{
public:
    static GmlServices &Get();

    void Init(std::unique_ptr<XmlConfig> gmlCfg, std::map<std::string, std::unique_ptr<XsdConfig>> xsdCfgs);

    bool IsInitialized() const { return initialized; }

    void PerformCreateGml(const std::filesystem::path &filePath);
    void PerformImport(const std::filesystem::path &inFile, std::unique_ptr<GmlObject> &importedObject);
    void PerformExport(const std::unique_ptr<GmlObject> &exportObject);

    void PerformDivision(const std::filesystem::path &inFile, std::vector<NamespacePrefix> &nsVec, bool isZip);
    void PerformMerge(const FilePath &inFile, std::vector<FilePath> &filePathVec);

    std::map<NamespacePrefix, std::map<GmlId, std::vector<GmlId>>> GetReferencesTo(const std::unique_ptr<GmlObject> &sourceObject, const std::map<NamespacePrefix, std::vector<std::string>> &classMap);
    std::map<NamespacePrefix, std::vector<GmlId>> GetReferencesFrom(const std::unique_ptr<GmlObject> &sourceObject, std::map<std::string, std::map<GmlId, std::vector<GmlId>>> &referencesMap);
    std::map<NamespacePrefix, std::vector<GmlId>> GetReferencesFromConfig(const std::unique_ptr<GmlObject> &sourceObject);

    std::map<NamespacePrefix, std::vector<GmlId>> GetElementsFromClasses(const std::unique_ptr<GmlObject> &sourceObject, const std::map<std::string, std::vector<std::string>> &classes);

    std::map<NamespacePrefix, std::vector<GmlId>> GetElementsFromQuery(const std::unique_ptr<GmlObject> &sourceObject, const std::string &query_type, const std::string &query_request);
private:
    GmlServices() = default;

    bool initialized = false;

    std::unique_ptr<XmlConfig> gmlCfg;
    std::map<std::string, std::unique_ptr<XsdConfig>> xsdCfgs;
};