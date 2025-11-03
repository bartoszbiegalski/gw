#pragma once

#include "core/types.h"
#include "core/Object.h"
#include "exceptions/Exceptions.h"
#include "services/XmlConfig.h"

class XmlParser
{
public:
    XmlParser() = delete;
    ~XmlParser() = delete;

    static void SetContent(const std::unique_ptr<XmlConfig> &cfg, std::unique_ptr<Object> &obj);

private:
};