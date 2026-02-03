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

nlohmann::json XsdConfig::get_json(const std::string &key) const
{
    return data_.contains(key) ? data_.at(key) : nlohmann::json{};
}
