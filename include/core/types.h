#pragma once
/**
 * @brief Header file for every typedef, namespace method etc.
 */

#include <libxml/xmlreader.h>

#include <string>
#include <fstream>
#include <filesystem>
#include <optional>
#include <algorithm>

#include <string.h>

#include <list>
#include <map>
#include <vector>
#include <unordered_set>

using FileName = std::string;
using FilePath = std::filesystem::path;

using NamespacePrefix = std::string;
using GmlId = std::string;

// operator do stringa zeby wyciagal prefix lub suffix

struct NamespaceData
{
    std::string NamespaceUri;
    std::string NamespaceUrl;
};

struct XmlNodeDeleter
{
    void operator()(xmlNode *node) const noexcept
    {
        if (node)
            xmlFreeNode(node);
    }
};

using GmlNodePtr = std::shared_ptr<xmlNode>;
using NamespaceMap = std::map<NamespacePrefix, NamespaceData>;
// using Entry = std::pair<GmlId, GmlNodePtr>;
using GmlMap = std::map<NamespacePrefix, std::map<GmlId, GmlNodePtr>>;
using XsdValidityMap = std::map<NamespacePrefix, bool>;

using XmlSchemaParserCtxtPtr = std::unique_ptr<_xmlSchemaParserCtxt, decltype(&xmlSchemaFreeParserCtxt)>;
using XmlSchemaPtr = std::unique_ptr<_xmlSchema, decltype(&xmlSchemaFree)>;
using XmlSchemaValidCtxtPtr = std::unique_ptr<_xmlSchemaValidCtxt, decltype(&xmlSchemaFreeValidCtxt)>;
