#pragma once

/**
 * @brief Class representing encapsulating gml data into class instance
 */

#include "core/types.h"
#include "core/GmlStorage.h"

class Object
{
public:
    Object();
    ~Object() = default;

    const FileName getFileName() const { return this->fileName; }
    void setFileName(const FileName &fileName) { this->fileName = fileName; }

    const FilePath getFilePath() const { return this->filePath; }
    void setFilePath(const FilePath &filePath) { this->filePath = filePath; }

    NamespaceMap &getNamespaceMap() { return this->namespaceMap; }
    const NamespaceMap &getNamespaceMap() const { return this->namespaceMap; }

    void setNamespaceMap(const NamespaceMap &namespaceMap) { this->namespaceMap = namespaceMap; }
    void addToNamespaceMap(const NamespacePrefix &namespacePrefix, const NamespaceData &namespaceData) { this->namespaceMap[namespacePrefix] = namespaceData; }

    GmlStorage &getGmlStorage() noexcept { return this->gmlStorage; }
    const GmlStorage &getGmlStorage() const noexcept { return this->gmlStorage; }
    void setGmlStorage(GmlStorage &gmlStorage) { this->gmlStorage = std::move(gmlStorage); }
    void addToGmlStorage(const NamespacePrefix &prefix, const GmlId &gmlId, GmlNodePtr nodePtr) { this->gmlStorage.add(prefix, gmlId, std::move(nodePtr)); }
    bool hasNamespace(const NamespacePrefix &prefix) { return this->gmlStorage.hasNamespace(prefix); }

    void setXmlValidity(bool isValid) { this->isXmlValid = isValid; }
    bool getXmlValidity() const { return this->isXmlValid; }

    void setXsdValidity(bool isValid) { this->isXsdValid = isValid; }
    bool getXsdValidity() const { return this->isXsdValid; }

private:
    bool isXmlValid = false;
    bool isXsdValid = false;

    FileName fileName;
    FilePath filePath;

    NamespaceMap namespaceMap;
    GmlStorage gmlStorage;
};