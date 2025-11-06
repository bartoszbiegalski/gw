#include "core/GmlStorage.h"

void GmlStorage::add(const NamespacePrefix &ns, const GmlId &id, GmlNodePtr node)
{
    gmlMap[ns].emplace(id, std::move(node));
}

std::optional<xmlNode *> GmlStorage::findById(const GmlId &id) const
{
    for (const auto &[ns, map] : gmlMap)
    {
        auto it = std::find_if(map.begin(), map.end(),
                               [&id](auto const &pair)
                               { return pair.first == id; });
        if (it != map.end())
            return it->second.get();
    }
    return std::nullopt;
}

std::vector<xmlNode *> GmlStorage::findInNamespace(const NamespacePrefix &ns) const
{
    std::vector<xmlNode *> result;
    auto it = gmlMap.find(ns);
    if (it == gmlMap.end())
        return result;

    result.reserve(it->second.size());
    for (const auto &[id, ptr] : it->second)
        result.push_back(ptr.get());
    return result;
}

bool GmlStorage::hasNamespace(const NamespacePrefix &prefix)
{
    return gmlMap.at(prefix).empty() ? false : true;
};

bool GmlStorage::removeById(const GmlId &id)
{
    for (auto &[ns, map] : gmlMap)
    {
        for (auto it = map.begin(); it != map.end();)
        {
            if (it->first == id)
            {
                it = map.erase(it);
                return true;
            }
            else
            {
                ++it;
            }
        }
    }
    return false;
}

void GmlStorage::removeNamespace(const NamespacePrefix &ns)
{
    gmlMap.erase(ns);
}

bool GmlStorage::contains(const GmlId &id) const
{
    return findById(id).has_value();
}

void GmlStorage::setXsdValidity(const NamespacePrefix &ns, bool validity)
{
    if (gmlMap.find(ns) != gmlMap.end())
    {
        xsdValidityMap[ns] = validity;
    }
}
bool GmlStorage::getXsdValidity(const NamespacePrefix &ns) const
{
    if (xsdValidityMap.find(ns) != xsdValidityMap.end())
    {
        return xsdValidityMap.at(ns);
    }
    return false;
}
