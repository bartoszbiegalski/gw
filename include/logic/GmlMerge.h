#pragma once

#include "core/types.h"
#include "core/GmlObject.h"
#include "utils/string_operations.h"
#include "services/XmlConfig.h"

class GmlMerge
{
public:
    static void Merge(const std::unique_ptr<XmlConfig> &cfg, const std::vector<std::unique_ptr<GmlObject>> &baseObjects, std::unique_ptr<GmlObject> &mergedObject);
    static void MergeOne(const std::unique_ptr<XmlConfig> &cfg, const std::unique_ptr<GmlObject> &sourceObject, std::unique_ptr<GmlObject> &mergedObject);
};
