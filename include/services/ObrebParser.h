#pragma once

#include <fstream>
#include <filesystem>

#include "core/types.h"

class ObrebParser
{
public:
    ObrebParser(const std::filesystem::path &filePath)
        : filePath(filePath) {}

    std::string GetObrebName(const std::string &obrebTeryt);
    std::string GetJednostkaName(const std::string &jednostkaTeryt);

private:
    std::filesystem::path filePath;
};