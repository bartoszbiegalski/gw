#pragma once

#include "core/GmlObject.h"
#include "core/types.h"
#include "services/XmlConfig.h"
#include "utils/NamespaceTool.h"
#include "utils/string_operations.h"
#include "utils/tree_operations.h"

class GmlExport
{
public:
    GmlExport() = delete;

    static void Export(const std::unique_ptr<XmlConfig> &cfg, const std::unique_ptr<GmlObject> &obj);

private:
    static void writeNode(xmlTextWriterPtr writer, xmlNodePtr node, bool isRoot);
};