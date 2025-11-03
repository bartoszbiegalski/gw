#include "services/XsdConfig.h"

XsdConfig::XsdConfig(const FilePath &filePath)
{
    std::ifstream f(filePath.string());
    if (f)
        f >> data_;
}

std::string XsdConfig::get(const std::string &key, const std::string &def) const
{
    return data_.value(key, def);
}