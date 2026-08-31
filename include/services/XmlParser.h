#pragma once

#include "core/types.h"
#include "core/GmlObject.h"
#include "exceptions/Exceptions.h"
#include "services/XmlConfig.h"
#include "utils/tree_operations.h"

class XmlParser
{
public:
    XmlParser() = delete;
    ~XmlParser() = delete;

    static void SetContent(const std::unique_ptr<XmlConfig> &cfg, std::unique_ptr<GmlObject> &obj);

private:
};