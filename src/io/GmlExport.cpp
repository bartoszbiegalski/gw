#include "exceptions/Exceptions.h"
#include "io/GmlExport.h"

#include <fstream>
#include <iostream>

void GmlExport::Export(const std::unique_ptr<XmlConfig> &cfg, const std::unique_ptr<Object> &obj)
{
    int idNum = 1;
    xmlBufferPtr buf = xmlBufferCreate();

    std::string gmlVersion{cfg.get()->get("gml_preambule.gml_version", "")};
    std::string enconding{cfg.get()->get("gml_preambule.gml_encoding", "")};
    std::string gmlPrefix{cfg.get()->get("gml_prefix", "gml")};

    std::string sep{cfg.get()->get("gml_separator", ":")};
    std::string nsPrefix{cfg.get()->get("namespace_prefix", "xmlns")};

    std::string rootName{gmlPrefix + sep + cfg.get()->get("gml_structure.root", "")};
    std::string childName{gmlPrefix + sep + cfg.get()->get("gml_structure.child", "")};

    xmlDocPtr doc = xmlNewDoc(BAD_CAST gmlVersion.c_str());
    xmlNodePtr root = xmlNewNode(nullptr, BAD_CAST rootName.c_str());

    xmlDocSetRootElement(doc, root);

    std::string idAttr{cfg.get()->get("id_attribute", "")};
    std::string idValue{cfg.get()->get("id_value", "")};

    xmlNewProp(root, BAD_CAST idAttr.c_str(), BAD_CAST(idValue + std::to_string(idNum)).c_str());
    idNum++;

    for (const auto &[prefix, value] : obj.get()->getNamespaceMap())
    {
        xmlNewProp(root, BAD_CAST std::string(nsPrefix + sep + prefix).c_str(), BAD_CAST value.NamespaceUri.c_str());
    }

    std::list<std::string> namespaceList;
    NamespaceTool::MapToNamespaceList(obj.get()->getNamespaceMap(), namespaceList);

    std::string schemaLocationAttr{cfg.get()->get("schema_location_attribute", "")};
    std::string schemaLocationValue;
    string_operations::set_schema_location_string(namespaceList, schemaLocationValue);

    xmlNewProp(root, BAD_CAST schemaLocationAttr.c_str(), BAD_CAST schemaLocationValue.c_str());

    // children

    for (const auto &child : obj.get()->getGmlStorage().getGmlMap())
    {
        for (const auto &xmlObj : child.second)
        {
            xmlNodePtr copy = xmlDocCopyNode(xmlObj.second.get(), doc, 1);
            // szukamy id
            auto foundNode = tree_operations::get_xmlNode_with_attr(copy->children, string_operations::get_suffix(idAttr, sep), string_operations::get_prefix(idAttr, sep));
            if (foundNode != nullptr)
            {
                // std::cout << foundNode->name << " " << idNum << '\n';
                xmlSetProp(foundNode, BAD_CAST "gml:id", BAD_CAST(idValue + std::to_string(idNum)).c_str());
                idNum++;
            }

            xmlNodePtr rootChild = xmlNewNode(nullptr, BAD_CAST childName.c_str());
            xmlAddChild(rootChild, copy);
            xmlAddChild(root, rootChild);
        }
    }
    std::cout << obj->getFileName() << "\n";
    xmlSaveFormatFileEnc(obj->getFileName().c_str(), doc, enconding.c_str(), 1);
}