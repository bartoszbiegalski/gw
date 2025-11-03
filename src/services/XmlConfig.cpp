#include "services/XmlConfig.h"

XmlConfig::XmlConfig(const FilePath &filePath)
{
    std::ifstream f(filePath.string());
    if (f)
        f >> data_;
}

std::string XmlConfig::get(const std::string &key, const std::string &def) const
{
    const nlohmann::json *current = &data_;
    std::stringstream ss(key);
    std::string token;

    while (std::getline(ss, token, '.'))
    {
        if (current->contains(token))
        {
            current = &(*current)[token];
        }
        else
        {
            return def;
        }
    }

    if (current->is_string())
        return current->get<std::string>();
    else if (current->is_number())
        return std::to_string(current->get<int>());
    else
        return def;
}

nlohmann::json XmlConfig::get_json(const std::string &key) const
{
    const nlohmann::json *current = &data_;
    std::stringstream ss(key);
    std::string token;

    while (std::getline(ss, token, '.'))
    {
        if (current->contains(token))
        {
            current = &(*current)[token];
        }
        else
        {
            return nlohmann::json{}; // pusta mapa
        }
    }

    return *current;
}
