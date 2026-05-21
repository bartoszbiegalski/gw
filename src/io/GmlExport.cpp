#include "exceptions/Exceptions.h"
#include "io/GmlExport.h"

#include <fstream>
#include <iostream>

void GmlExport::writeNode(xmlTextWriterPtr writer, xmlNodePtr node, bool isRoot)
{
    if (!node || !writer)
        return;

    if (node->type == XML_ELEMENT_NODE)
    {
        const xmlChar *prefix = nullptr;
        const xmlChar *href = nullptr;

        // Deklarujemy namespace tylko dla root
        if (isRoot && node->ns)
        {
            prefix = node->ns->prefix;
            href = node->ns->href;
        }
        xmlTextWriterStartElementNS(writer, prefix, node->name, nullptr);

        //  Atrybuty – namespace dodajemy tylko dla root
        for (xmlAttrPtr attr = node->properties; attr; attr = attr->next)
        {
            const xmlChar *attrPrefix = nullptr;
            const xmlChar *attrHref = nullptr;
            if (isRoot && attr->ns)
            {
                attrPrefix = attr->ns->prefix;
                attrHref = attr->ns->href;
            }
            xmlChar *content = xmlNodeGetContent(attr->children);
            xmlTextWriterWriteAttributeNS(writer, attrPrefix, attr->name, nullptr, content);
            xmlFree(content);
        }
        // Dzieci rekurencyjnie
        for (xmlNodePtr child = node->children; child; child = child->next)
        {
            if (child->type == XML_TEXT_NODE)
                xmlTextWriterWriteString(writer, child->content);
            else
            {
                writeNode(writer, child, true);
            }
        }

        xmlTextWriterEndElement(writer);
    }
}

void GmlExport::Export(const std::unique_ptr<XmlConfig> &cfg, const std::unique_ptr<GmlObject> &obj)
{
    int idNum = 1;

    std::string gmlVersion{cfg->get("gml_preambule.gml_version", "")};
    std::string encoding{cfg->get("gml_preambule.gml_encoding", "")};
    std::string gmlPrefix{cfg->get("gml_prefix", "gml")};
    std::string sep{cfg->get("gml_separator", ":")};
    std::string nsPrefix{cfg->get("namespace_prefix", "xmlns")};

    std::string rootName{gmlPrefix + sep + cfg->get("gml_structure.root", "")};
    std::string idAttr{cfg->get("id_attribute", "")};
    std::string idValue{cfg->get("id_value", "")};

    xmlTextWriterPtr writer = xmlNewTextWriterFilename(obj->getFilePath().u8string().c_str(), 0);
    xmlTextWriterSetIndent(writer, 0);

    xmlTextWriterStartDocument(writer, gmlVersion.c_str(), encoding.c_str(), nullptr);
    //xmlTextWriterWriteComment(writer, BAD_CAST obj->getComment().c_str());

    // Tworzymy root
    xmlTextWriterStartElementNS(writer, nullptr, BAD_CAST rootName.c_str(), nullptr);

    // Atrybuty root
    xmlTextWriterWriteAttribute(writer,
                                BAD_CAST idAttr.c_str(),
                                BAD_CAST(idValue + std::to_string(idNum)).c_str());
    idNum++;

    for (auto &nsPair : obj->getNamespaceMap())
    {
        xmlTextWriterWriteAttribute(writer,
                                    BAD_CAST(nsPrefix + sep + nsPair.first).c_str(),
                                    BAD_CAST nsPair.second.NamespaceUri.c_str());
    }

    std::list<std::string> namespaceList;
    NamespaceTool::MapToNamespaceList(obj->getNamespaceMap(), namespaceList);

    std::string schemaLocationAttr{cfg->get("schema_location_attribute", "")};
    std::string schemaLocationValue;
    string_operations::set_schema_location_string(namespaceList, schemaLocationValue);

    xmlTextWriterWriteAttribute(writer,
                                BAD_CAST schemaLocationAttr.c_str(),
                                BAD_CAST schemaLocationValue.c_str());
    xmlTextWriterWriteRaw(writer, BAD_CAST "\n  "); // ręczne wcięcie

    // Wpisujemy featureMember i jego dzieci
    for (const auto &childPair : obj->getGmlStorage().getGmlMap())
    {
        for (const auto &xmlObj : childPair.second)
        {
            xmlTextWriterStartElement(writer, BAD_CAST(gmlPrefix + sep + "featureMember").c_str());
            xmlTextWriterWriteRaw(writer, BAD_CAST "\n  ");

            xmlNodePtr node = xmlObj.second.get();
            std::cout<<node->name<<'\n';
            // zmiana gml:id
            auto idNodesVector = std::vector<xmlNodePtr>();
            tree_operations::get_xmlNodes_with_attr(
                node->children, 
                idNodesVector,
                string_operations::get_suffix(idAttr, sep),
                string_operations::get_prefix(idAttr, sep));

            for (auto foundNode : idNodesVector)
            {
                xmlSetProp(foundNode, BAD_CAST "gml:id", BAD_CAST((idValue + std::to_string(idNum)).c_str()));
                idNum++;
            }

            writeNode(writer, node, true);

            xmlTextWriterWriteRaw(writer, BAD_CAST "\n  ");

            xmlTextWriterEndElement(writer);
            xmlTextWriterWriteRaw(writer, BAD_CAST "\n  ");
        }
    }

    xmlTextWriterEndElement(writer); // koniec FeatureCollection
    xmlTextWriterEndDocument(writer);

    xmlFreeTextWriter(writer);
}
