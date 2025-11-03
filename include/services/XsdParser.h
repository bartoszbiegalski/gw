#pragma once

#include "core/Object.h"
#include "core/types.h"
#include "exceptions/Exceptions.h"
#include "services/XmlConfig.h"
#include "utils/NamespaceTool.h"

class XsdParser
{
public:
    XsdParser() = delete;
    ~XsdParser() = delete;

    static void Parse(std::unique_ptr<XmlConfig> &cfg, std::unique_ptr<Object> &obj);

private:
    static XmlSchemaValidCtxtPtr getValidCtxt(const std::string &xsdPath);
};