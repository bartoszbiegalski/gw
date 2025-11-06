#pragma once

#include "core/types.h"
#include "core/Object.h"
#include "services/XmlConfig.h"

class GmlCreate
{
public:
    GmlCreate() = delete;
    // tworzy pustego gml-a w podanej lokalizacji i o podanej nazwie
    static void Create(const std::unique_ptr<XmlConfig> &cfg, const FilePath &filePath, const FileName &fileName);
};