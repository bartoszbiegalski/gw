#pragma once

#include "core/types.h"
#include "core/GmlObject.h"

class GmlImport
{
public:
    GmlImport() = delete;

    static void Import(const FilePath &filePath, std::unique_ptr<GmlObject> &obj);
};