#include "utils/tree_operations.h"
#include <iostream>

namespace tree_operations
{
    GmlNodePtr &get_gml_node(const GmlNodePtr &gmlNodePtr, const std::string name)
    {
        // auto tempNode = gmlNodePtr.get();
        // get_xmlNode(tempNode, name);
    }

    // GmlNodePtr &get_gml_node(const GmlNodePtr &gmlNodePtr, const std::list<std::string, std::string> attrList)
    // {
    // }
    // we traverse through tree to get the node with corresponding name or nullptr

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
}