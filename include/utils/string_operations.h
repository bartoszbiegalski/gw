#pragma once

#include "core/types.h"

namespace string_operations
{
    std::string get_prefix(const std::string &key, const std::string &sep);

    std::string get_suffix(const std::string &key, const std::string &sep);

    std::list<std::string> get_schema_locaton_list(std::string &schemaLocationStream);

    bool validate_schemaLocation_stream(const std::string &schemaLocationStream);

    void set_schema_location_list(const std::string &schemaLocationStream, std::list<std::string> &schemaLocationList);

    void set_schema_location_string(const std::list<std::string> &schemaLocationList, std::string &schemaLocationStream);
}
