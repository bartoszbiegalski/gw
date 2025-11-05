#pragma once
#include "core/types.h"
#include <zip.h>

class ZipSaver
{
public:
    ZipSaver() = delete;
    static void SaveToZip(const std::string &folderName, const std::filesystem::path &folderPath, const std::vector<std::string> &fileVector);
};