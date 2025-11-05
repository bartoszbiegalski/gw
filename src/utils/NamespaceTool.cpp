#include "core/Object.h"
#include "core/types.h"

#include "exceptions/Exceptions.h"
#include "services/XmlConfig.h"
#include "utils/NamespaceTool.h"
#include "utils/string_operations.h"

#include <iostream>

std::map<std::string, std::string> NamespaceTool::GetAttributesMap(const std::unique_ptr<XmlConfig> &cfg, const std::unique_ptr<Object> &obj)
{
    xmlTextReaderPtr reader = xmlReaderForFile(obj->getFilePath().u8string().c_str(), nullptr, 0);
    if (!reader)
    {
        throw FileNotFoundException(obj->getFilePath().u8string());
    }

    /// uzywamy cfg do sprawdzania elementow gml-a
    std::string cfgSeparator = cfg.get()->get("gml_separator", ":");
    auto foundAttributesMap = std::map<std::string, std::string>();

    bool foundComment, foundRootAttributes = false;

    while (xmlTextReaderRead(reader) == 1)
    {
        if (foundRootAttributes == true && foundComment == true)
            break;

        if (xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT)
        {
            int attrCount = xmlTextReaderAttributeCount(reader);
            for (int i = 0; i < attrCount; ++i)
            {
                xmlTextReaderMoveToAttributeNo(reader, i);
                std::string prefix = reinterpret_cast<const char *>(xmlTextReaderPrefix(reader));
                std::string name = reinterpret_cast<const char *>(xmlTextReaderConstLocalName(reader));
                std::string attrValue = reinterpret_cast<const char *>(xmlTextReaderConstValue(reader));

                std::string attrName = prefix + cfgSeparator + name;
                foundAttributesMap.emplace(attrName, attrValue);
            }
            foundRootAttributes = true;
        }
        else if (xmlTextReaderNodeType(reader) == XML_READER_TYPE_COMMENT)
        {
            const xmlChar *comment = xmlTextReaderConstValue(reader);
            if (comment)
            {
                obj.get()->setComment(reinterpret_cast<const char *>(comment));
            }
            foundComment = true;
        }
    }
    xmlTextReaderClose(reader);
    return foundAttributesMap;
}
/**
 * Checks if attributes reqiured for xml-work purpoces are provided in &attributesMap
 */
std::vector<std::string> NamespaceTool::GetMissingAttributes(const std::unique_ptr<XmlConfig> &cfg, const std::map<std::string, std::string> &attributesMap)
{
    auto requiredAttributesMap = cfg.get()->get_json("gml_structure.required_attributes");
    std::vector<std::string> missingAttributesVector{};

    for (const auto &reqAttr : requiredAttributesMap)
    {
        std::string attrName = {reqAttr.get<std::string>()};

        auto it = std::find_if(attributesMap.begin(), attributesMap.end(),
                               [&attrName](const auto &mapElement)
                               { return mapElement.first == attrName; });

        if (it == attributesMap.end())
        {
            std::cout << "Brak wymaganego atrybutu: " << attrName << "\n";
            missingAttributesVector.push_back(attrName);
        }
    }
    return missingAttributesVector;
}

void NamespaceTool::NamespaceListToMap(const std::list<std::string> &namespaceUrlList, NamespaceMap &nsMap)
{
    for (auto it = namespaceUrlList.begin(); it != namespaceUrlList.end();)
    {
        auto next = it;
        std::advance(next, 1);

        if (next == namespaceUrlList.end())
            break;

        for (auto i : nsMap)
        {
            if (i.second.NamespaceUri == *it)
            {
                nsMap[i.first].NamespaceUrl = *next;
            }
        }

        std::advance(it, 2);
    }
}

void NamespaceTool::MapToNamespaceList(const NamespaceMap &nsMap, std::list<std::string> &namespaceUrlList)
{
    for (auto &[prefix, nsData] : nsMap)
    {
        if (nsData.NamespaceUrl != "")
        {
            namespaceUrlList.push_back(nsData.NamespaceUri);
            namespaceUrlList.push_back(nsData.NamespaceUrl);
        }
    }
}

NamespaceMap NamespaceTool::CreateNamespaceMap(const std::unique_ptr<XmlConfig> &cfg, std::map<std::string, std::string> &attributesMap)
{
    NamespaceMap nsMap{};
    std::list<std::string> namespaceUrlList{};

    std::string sep = cfg.get()->get("gml_separator", "error");
    // przechodzimy po attributesMap i sprawdzamy prefix: jesli gml:id to dajemy 1, jesli xmlns to zapisujemy do namespacecostam, wczesiej bierzemy xsischemalocation.
    // potem stworzone namespace costam i
    for (auto it : attributesMap)
    {
        if (cfg.get()->get("id_attribute", "") == it.first)
        {
            // gml:id, zostawiamy
        }
        else if (cfg.get()->get("schema_location_attribute", "") == it.first)
        {
            /// mamy xsi::schemaLocation, ustawiamy sobie liste ze stringa
            string_operations::set_schema_location_list(it.second, namespaceUrlList);
        }
        else if (cfg.get()->get("namespace_prefix", "") == string_operations::get_prefix(it.first, sep))
        {
            nsMap.emplace(string_operations::get_suffix(it.first, sep), NamespaceData{it.second, ""});
            // mamy xmlns:costam, dodajemy do listy
        }
    }
    NamespaceListToMap(namespaceUrlList, nsMap);
    return nsMap;
}

std::vector<std::pair<std::string, std::string>> NamespaceTool::GetXsdVector(const std::unique_ptr<Object> &obj)
{
    auto xsdFileVector = std::vector<std::pair<std::string, std::string>>();
    for (const auto &it : obj.get()->getNamespaceMap())
    {
        if (obj.get()->getGmlStorage().getGmlMap().find(it.first) != obj.get()->getGmlStorage().getGmlMap().end())
        {
            xsdFileVector.emplace_back(it.first, it.second.NamespaceUrl);
        }
    }
    return xsdFileVector;
}

void NamespaceTool::Process(const std::unique_ptr<XmlConfig> &cfg, std::unique_ptr<Object> &obj)
{
    auto attributesMap = GetAttributesMap(cfg, obj);

    if (GetMissingAttributes(cfg, attributesMap).empty() == false)
    {
        // jakis error
        std::cout << "kurcze niedobrze\n";
    }
    else
    {
        auto nsMap = CreateNamespaceMap(cfg, attributesMap);
        obj->setNamespaceMap(std::move(nsMap));
    }
}