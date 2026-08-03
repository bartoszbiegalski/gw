/**
 * @file Object.cpp
 * @brief Definitions of Object class
 */

#include "core/GmlObject.h"

GmlObject::GmlObject() {}

int GmlObject::getElementAmount()
{
    return std::accumulate(getGmlStorage().getGmlMap().begin(),
                           getGmlStorage().getGmlMap().end(), 0,
                           [&](int acc, const auto &pair)
                           {
                               return acc + pair.second.size();
                           });
}

int GmlObject::getNamespaceElementAmount(const NamespacePrefix &prefix)
{
    if (getGmlStorage().getGmlMap().find(prefix) == getGmlStorage().getGmlMap().end())
        return 0;
    else
        return getGmlStorage().getGmlMap().at(prefix).size();
}

std::map<std::string, int> GmlObject::GetClassNames(const NamespacePrefix &prefix)
{
    std::map<std::string, int> classNameMap{};

    const auto &gmlMap = getGmlStorage().getGmlMap();
    auto found = gmlMap.find(prefix);
    if (found == gmlMap.end())
        return classNameMap;

    const auto &entries = found->second;

    for (const auto &[nsPrefix, vector] : entries)
    {
        for (auto it = vector.get(); it != nullptr; it = it->next)
        {
            if (it->type == XML_TEXT_NODE || it->name == nullptr)
                continue;

            std::string nodeName(reinterpret_cast<const char *>(it->name));

            classNameMap[nodeName]++;
        }
    }

    return classNameMap;
}

