#include "services/XmlParser.h"

#include <iostream>

void XmlParser::SetContent(const std::unique_ptr<XmlConfig> &cfg, std::unique_ptr<Object> &obj)
{
    xmlInitParser();
    xmlDocPtr doc = xmlReadFile(obj->getFilePath().string().c_str(), nullptr, 0);

    if (!doc)
    {
        throw XmlParseException(obj->getFileName());
    }
    obj.get()->setXmlValidity(true);

    GmlStorage gmlStorage{};

    xmlNode *root = xmlDocGetRootElement(doc);
    if (root && cfg.get()->get("gml_structure.root", "") == reinterpret_cast<const char *>(root->name))
    {
        for (xmlNode *child = root->children; child != nullptr; child = child->next)
        {
            if (reinterpret_cast<const char *>(child->name) == cfg.get()->get("gml_structure.child", ""))
            {
                for (xmlNode *childChild = child->children; childChild != nullptr; childChild = childChild->next)
                {
                    std::string prefix;
                    if (childChild->ns && childChild->ns->prefix)
                        prefix = reinterpret_cast<const char *>(childChild->ns->prefix);
                    else
                        prefix = ""; // np. pusty prefix dla domyślnej przestrzeni nazw
                    if (prefix != "")
                    {
                        std::string id = reinterpret_cast<const char *>(childChild->properties->children->content);
                        gmlStorage.add(prefix, id, GmlNodePtr(childChild, XmlNodeDeleter{}));
                    }
                }
            }
        }
    }
    obj->setGmlStorage(gmlStorage);
    xmlCleanupParser();
}
