#pragma once

#include "core/types.h"
#include "external/json.hpp"
#include "services/Config.h"

#include <fstream>

class XmlConfig : public Config
{
public:
    XmlConfig(const FilePath &filePath);

    XmlConfig(const nlohmann::json newData);

    std::string get(const std::string &key, const std::string &def) const override;

    nlohmann::json get_json(const std::string &key) const override;
};
