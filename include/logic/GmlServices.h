#pragma once

#include <libxml/tree.h>

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
#include "logic/GmlSpatialServices.h"

#include <geos/operation/valid/IsValidOp.h>

class GmlServices
{
public:
    static GmlServices &Get();

    void Init(std::unique_ptr<XmlConfig> gmlCfg, std::map<std::string, std::unique_ptr<XsdConfig>> xsdCfgs);

    bool IsInitialized() const { return initialized; }

    void PerformCreateGml(const std::filesystem::path &filePath);
    void PerformImport(const std::filesystem::path &inFile, std::unique_ptr<GmlObject> &importedObject);
    void PerformExport(const std::unique_ptr<GmlObject> &exportObject);

    void PerformDivision(const FilePath &inFile, std::vector<NamespacePrefix> &nsVec, bool isZip);
    void PerformDivisionJ(const FilePath &inFile, bool isRaport);
    void PerformDivisionO(const FilePath &inFile, bool isRaport);
    void PerformDivisionWithIdVector(const FilePath &inFile, const std::string &fileSuffix, std::vector<GmlId> &idVector, bool isZip);

    void PerformMerge(const FilePath &inFile, std::vector<FilePath> &filePathVec);

    std::map<NamespacePrefix, std::map<GmlId, std::vector<GmlId>>> GetReferencesTo(const std::unique_ptr<GmlObject> &sourceObject, const std::map<NamespacePrefix, std::vector<ClassName>> &classMap);
    std::map<NamespacePrefix, std::map<GmlId, std::vector<GmlId>>> GetReferencesTo(const std::unique_ptr<GmlObject> &sourceObject, const std::vector<GmlId> &vecId);
    std::map<NamespacePrefix, std::vector<GmlId>> GetReferencesFrom(const std::unique_ptr<GmlObject> &sourceObject, std::map<NamespacePrefix, std::map<GmlId, std::vector<GmlId>>> &referencesMap);
    std::map<NamespacePrefix, std::vector<GmlId>> GetReferencesFromConfig(const std::unique_ptr<GmlObject> &sourceObject);

    std::map<NamespacePrefix, std::vector<GmlId>> GetElementsFromClass(const std::unique_ptr<GmlObject> &sourceObject, const NamespacePrefix &prefix, const ClassName &className);
    std::map<NamespacePrefix, std::vector<GmlId>> GetElementsFromClasses(const std::unique_ptr<GmlObject> &sourceObject, const std::map<NamespacePrefix, std::vector<ClassName>> &classes);

    GmlTreeModel GetJO(const std::unique_ptr<GmlObject> &sourceObject, const std::string &type, std::vector<GmlId> &reszta);

    std::map<GmlId, std::set<GmlId>> GetElementsFromQuery(const std::unique_ptr<GmlObject> &sourceObject, const std::string &query_type, const std::string &query_request, std::map<GmlId, std::pair<std::string, std::string>> &extra_attributes);
    const std::unique_ptr<XmlConfig> &GetGmlCfg() { return gmlCfg; }

private:
    GmlServices() = default;

    bool initialized = false;

    std::unique_ptr<XmlConfig> gmlCfg;
    std::map<std::string, std::unique_ptr<XsdConfig>> xsdCfgs;
};