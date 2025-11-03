#pragma once

#include "core/types.h"
#include "external/json.hpp"

class Config
{
public:
    virtual ~Config() = default;
    virtual std::string get(const std::string &key, const std::string &def = "") const = 0;
    virtual nlohmann::json get_json(const std::string &key) const = 0;

protected:
    nlohmann::json data_;
};