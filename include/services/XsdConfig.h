#pragma once

#include "core/types.h"
#include "external/json.hpp"
#include "services/Config.h"

#include <fstream>

class XsdConfig : public Config
{
public:
    explicit XsdConfig(const FilePath &filePath);

    std::string get(const std::string &key, const std::string &def) const override;
};
