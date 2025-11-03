#pragma once

#include "core/types.h"
#include "core/Object.h"
#include "utils/string_operations.h"
#include "services/XmlConfig.h"

class GmlDivide
{
public:
    static void Divide(const std::unique_ptr<XmlConfig> &cfg, std::unique_ptr<Object> &obj, const std::vector<NamespacePrefix> vNamespaces, std::vector<std::unique_ptr<Object>> &dividedObjects);
};
