#include "utils/string_operations.h"

namespace string_operations
{
    std::string get_prefix(const std::string &key, const std::string &sep)
    {
        auto pos = key.find(sep);
        if (pos == std::string::npos)
            return ""; // brak delimitera - zwracamy pusty string lub cały string - zależnie od potrzeb
        return key.substr(0, pos);
    }

    std::string get_suffix(const std::string &key, const std::string &sep)
    {
        auto pos = key.find(sep);
        if (pos == std::string::npos || pos + 1 >= key.size())
            return ""; // brak delimitera lub brak sufiksu
        return key.substr(pos + 1);
    }

    bool validate_schemaLocation_stream(const std::string &schemaLocationStream)
    {
        std::string str = schemaLocationStream;

        std::istringstream iss(str);
        std::string attributeValue;
        std::list<std::string> schemaLocationList;

        while (iss >> attributeValue)
        {
            schemaLocationList.push_back(attributeValue);
        }

        if (schemaLocationList.size() % 2 != 0)
        {
            return false;
        }

        return true;
    }

    void set_schema_location_list(const std::string &schemaLocationStream, std::list<std::string> &schemaLocationList)
    {
        std::string str = schemaLocationStream;

        std::istringstream iss(str);
        std::string attributeValue;

        while (iss >> attributeValue)
        {
            schemaLocationList.push_back(attributeValue);
        }
    }

    void set_schema_location_string(const std::list<std::string> &schemaLocationList, std::string &schemaLocationStream)
    {
        for (const auto &it : schemaLocationList)
        {
            schemaLocationStream += it + " ";
        }
    }
}
