#pragma once
#include "core/types.h"
#include "minizip/zip.h"

class ZipSaver
{
public:
    ZipSaver() = delete;
    static void SaveToZip(const std::vector<std::string> fileVector);
};