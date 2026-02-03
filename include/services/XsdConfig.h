#pragma once

#include "core/types.h"
#include "external/nlohmann/json.hpp"
#include "services/Config.h"

#include <fstream>

class XsdConfig : public Config
{
public:
    XsdConfig(const FilePath &filePath);

    std::string get(const std::string &key, const std::string &def) const override;

    nlohmann::json get_json(const std::string &key) const override;
};
