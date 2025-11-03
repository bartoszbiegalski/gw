#pragma once

#include "core/types.h"
#include "core/Object.h"

class GmlImport
{
public:
    GmlImport() = delete;

    static void Import(const FilePath &filePath, std::unique_ptr<Object> &obj);
};