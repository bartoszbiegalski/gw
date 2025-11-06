#pragma once

#include "core/types.h"
#include "core/Object.h"
#include "utils/string_operations.h"
#include "services/XmlConfig.h"

class GmlMerge
{
public:
    static void Merge(const std::unique_ptr<XmlConfig> &cfg, const std::vector<std::unique_ptr<Object>> &baseObjects, std::unique_ptr<Object> &mergedObject);
    static void MergeOne(const std::unique_ptr<XmlConfig> &cfg, const std::unique_ptr<Object> &sourceObject, std::unique_ptr<Object> &mergedObject);
};
