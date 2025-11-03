#pragma once

#include "core/types.h"

class GmlStorage
{
public:
    void add(const NamespacePrefix &ns, const GmlId &id, GmlNodePtr node);

    std::optional<xmlNode *> findById(const GmlId &id) const;

    std::vector<xmlNode *> findInNamespace(const NamespacePrefix &ns) const;

    bool hasNamespace(const NamespacePrefix &prefix);

    bool removeById(const GmlId &id);

    void removeNamespace(const NamespacePrefix &ns);

    void setGmlMap(const GmlMap &gmlMap) { this->gmlMap = gmlMap; }

    const GmlMap &getGmlMap() const noexcept { return gmlMap; }
    GmlMap &getGmlMap() noexcept { return gmlMap; }

    bool contains(const GmlId &id) const;

    void clear() noexcept { gmlMap.clear(); }

    void setXsdValidity(const NamespacePrefix &ns, bool validity);

    bool getXsdValidity(const NamespacePrefix &ns) const;

    void getXsdAllValidity() const;

private:
    GmlMap gmlMap;
    XsdValidityMap xsdValidityMap;
};
