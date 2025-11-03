#pragma once

#include "core/types.h"
#include "core/Object.h"
#include "services/XmlConfig.h"

class NamespaceTool
{
public:
    NamespaceTool() = delete;
    ~NamespaceTool() = delete;
    NamespaceTool(const NamespaceTool &) = delete;
    NamespaceTool(NamespaceTool &&) = delete;

    static void Process(const std::unique_ptr<XmlConfig> &cfg, std::unique_ptr<Object> &obj);
    static void NamespaceListToMap(const std::list<std::string> &namespaceUrlList, NamespaceMap &nsMap);

    static void MapToNamespaceList(const NamespaceMap &nsMap, std::list<std::string> &namespaceUrlList);
    static std::vector<std::pair<std::string, std::string>> GetXsdVector(const std::unique_ptr<Object> &obj);

private:
    static std::map<std::string, std::string> GetAttributesMap(const std::unique_ptr<XmlConfig> &cfg, const std::unique_ptr<Object> &obj);

    static std::vector<std::string> GetMissingAttributes(const std::unique_ptr<XmlConfig> &cfg, const std::map<std::string, std::string> &attributesMap);

    static NamespaceMap CreateNamespaceMap(const std::unique_ptr<XmlConfig> &cfg, std::map<std::string, std::string> &attributesMap);
};