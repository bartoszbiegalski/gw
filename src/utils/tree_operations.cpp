#include "utils/tree_operations.h"
#include <iostream>

namespace tree_operations
{
    /**
      *  XML_ELEMENT_NODE=		1,
         XML_ATTRIBUTE_NODE=		2,
         XML_TEXT_NODE=		3,
      */

    xmlNodePtr get_xmlNode_with_attr(xmlNodePtr nodePtr, const std::string &attr, const std::string &prefix)
    {
        if (!nodePtr)
            return nullptr;

        // Sprawdzamy bieżący węzeł tylko jeśli jest elementem
        if (nodePtr->type == XML_ELEMENT_NODE && nodePtr->properties != nullptr)
        {
            for (xmlAttrPtr prop = nodePtr->properties; prop != nullptr; prop = prop->next)
            {
                if (prefix == "")
                {
                    // no prefix, just attr
                    if (prop->name && attr == reinterpret_cast<const char *>(prop->name))
                    {
                        return nodePtr;
                    }
                }
                else
                {
                    if (prop->name && prop->ns && attr == reinterpret_cast<const char *>(prop->name) && prefix == reinterpret_cast<const char *>(prop->ns->prefix))
                        return nodePtr;
                }
            }
        }

        if (nodePtr->children != nullptr)
        {
            xmlNodePtr found = get_xmlNode_with_attr(nodePtr->children, attr, prefix);
            if (found)
                return found;
        }

        if (nodePtr->next != nullptr)
        {
            xmlNodePtr found = get_xmlNode_with_attr(nodePtr->next, attr, prefix);
            if (found)
                return found;
        }

        return nullptr;
    }

    void traverse_gml_id(xmlNodePtr nodePtr, const std::string &name, const std::string &attr, std::vector<GmlId> &foundId, const std::string &prefix)
    {
        if (!nodePtr)
            return;

        // Sprawdzamy bieżący węzeł tylko jeśli jest elementem
        if (nodePtr->type == XML_ELEMENT_NODE)
        {
            if (prefix == "")
            {
                if (nodePtr->name && name == reinterpret_cast<const char *>(nodePtr->name) && nodePtr->content)
                {
                    foundId.push_back(std::string(reinterpret_cast<const char *>(nodePtr->content)));
                }
            }
            else
            {
                if (nodePtr->name && nodePtr->ns && name == reinterpret_cast<const char *>(nodePtr->name) && prefix == reinterpret_cast<const char *>(nodePtr->ns->prefix))
                {
                    for (xmlAttr *attrNode = nodePtr->properties; attrNode; attrNode = attrNode->next)
                    {
                        const char *name = reinterpret_cast<const char *>(attrNode->name);
                        xmlChar *value = xmlNodeListGetString(
                            nodePtr->doc,
                            attrNode->children,
                            1);

                        if (value && attr == reinterpret_cast<const char *>(name))
                        {
                            std::string val(reinterpret_cast<const char *>(value));
                            xmlFree(value);
                            foundId.push_back(val);
                        }
                    }
                }
            }
        }

        if (nodePtr->children != nullptr)
        {
            traverse_gml_id(nodePtr->children, name, attr, foundId, prefix);
        }

        if (nodePtr->next != nullptr)
        {
            traverse_gml_id(nodePtr->next, name, attr, foundId, prefix);
        }
    }

    void get_xmlNodes_with_attr(xmlNodePtr nodePtr, std::vector<xmlNodePtr> &foundXmlNodes, const std::string &attr, const std::string &prefix)
    {
        if (nodePtr->type == XML_ELEMENT_NODE && nodePtr->properties != nullptr)
        {
            for (xmlAttrPtr prop = nodePtr->properties; prop != nullptr; prop = prop->next)
            {
                if (prefix == "")
                {
                    if (prop->name && attr == reinterpret_cast<const char *>(prop->name))
                    {
                        foundXmlNodes.push_back(nodePtr);
                    }
                }
                else
                {
                    if (prop->name && prop->ns && attr == reinterpret_cast<const char *>(prop->name) && prefix == reinterpret_cast<const char *>(prop->ns->prefix))
                    {
                        foundXmlNodes.push_back(nodePtr);
                    }
                }
            }
        }
        auto childPtr = nodePtr->children;
        if (childPtr != nullptr)
        {
            get_xmlNodes_with_attr(childPtr, foundXmlNodes, attr, prefix);
        }

        auto nextPtr = nodePtr->next;
        if (nextPtr != nullptr)
        {
            get_xmlNodes_with_attr(nextPtr, foundXmlNodes, attr, prefix);
        }
    }
}