#pragma once

#include "core/types.h"
#include "external/json.hpp"
#include "services/Config.h"

#include <fstream>

class XmlConfig : public Config
{
public:
    explicit XmlConfig(const FilePath &filePath);

    std::string get(const std::string &key, const std::string &def) const override;

    nlohmann::json get_json(const std::string &key) const override;
};
