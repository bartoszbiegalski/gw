#pragma once

#include "core/types.h"
#include "core/GmlObject.h"
#include "utils/string_operations.h"
#include "utils/gml_operations.h"
#include "services/XmlConfig.h"

class GmlDivide
{
public:
    static void Divide(const std::unique_ptr<XmlConfig> &cfg, std::unique_ptr<GmlObject> &sourceObject, const std::vector<NamespacePrefix> &prefixes, std::vector<std::unique_ptr<GmlObject>> &dividedObjects);
    static void DivideJO(const std::unique_ptr<XmlConfig> &cfg, const GmlTreeModel &model, std::unique_ptr<GmlObject> &sourceObject, std::vector<std::unique_ptr<GmlObject>> &dividedObjects);
    static void DivideFromIdVector(const std::unique_ptr<XmlConfig> &cfg, std::unique_ptr<GmlObject> &sourceObject, const std::string &fileSuffix, const std::vector<GmlId> idVector, std::unique_ptr<GmlObject> &dividedObject);
};
