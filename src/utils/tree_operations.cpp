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
    void get_xmlNodes_with_names(xmlNodePtr nodePtr, std::vector<xmlNodePtr> &foundNodes, const std::vector<std::string> &names, const std::string &prefix)
    {
        if (nodePtr->type == XML_ELEMENT_NODE)
        {
            std::string newName = reinterpret_cast<const char *>(nodePtr->name);
            if (prefix == "")
            {
                if (std::find(names.begin(), names.end(), newName) != names.end())
                {
                    foundNodes.push_back(nodePtr);
                }
            }
            else
            {
                if (prefix == reinterpret_cast<const char *>(nodePtr->ns->prefix) && std::find(names.begin(), names.end(), newName) != names.end())
                {
                    foundNodes.push_back(nodePtr);
                }
            }
        }
        auto childPtr = nodePtr->children;
        if (childPtr != nullptr)
        {
            get_xmlNodes_with_names(childPtr, foundNodes, names, prefix);
        }

        auto nextPtr = nodePtr->next;
        if (nextPtr != nullptr)
        {
            get_xmlNodes_with_names(nextPtr, foundNodes, names, prefix);
        }
    }

    xmlNodePtr get_xmlNode_from_name(xmlNodePtr nodePtr, const std::string &name, const std::string &prefix)
    {
        if (nodePtr->type == XML_ELEMENT_NODE)
        {
            if (prefix == "")
            {
                if (name == reinterpret_cast<const char *>(nodePtr->name))
                {
                    return nodePtr;
                }
            }
            else
            {
                if (name == reinterpret_cast<const char *>(nodePtr->name) && prefix == reinterpret_cast<const char *>(nodePtr->ns->prefix))
                {
                    return nodePtr;
                }
            }
        }
        auto childPtr = nodePtr->children;
        if (childPtr != nullptr)
        {
            xmlNodePtr found = get_xmlNode_from_name(childPtr, name, prefix);
            if (found)
            {
                return found;
            }
        }

        auto nextPtr = nodePtr->next;
        if (nextPtr != nullptr)
        {
            xmlNodePtr found = get_xmlNode_from_name(nextPtr, name, prefix);
            if (found)
            {
                return found;
            }
        }

        return nullptr;
    }

    xmlNodePtr get_xmlNodes_name_value_pattern(xmlNodePtr nodePtr, const std::string &namePattern, const std::string &valuePattern, const std::string &prefix)
    {
        if (!nodePtr)
            return nullptr;

        if (nodePtr->type == XML_ELEMENT_NODE)
        {
            std::string name = reinterpret_cast<const char *>(nodePtr->name);
            if (name.find(namePattern) != std::string::npos && nodePtr->children != nullptr)
            {
                std::string value = reinterpret_cast<const char *>(nodePtr->children->content);
                std::regex r(valuePattern);

                if (std::regex_match(value, r))
                {
                    if (prefix == "")
                        return nodePtr;

                    if (nodePtr->ns && prefix == reinterpret_cast<const char *>(nodePtr->ns->prefix))
                        return nodePtr;
                }
            }
        }
        if (nodePtr->children)
        {
            if (auto res = get_xmlNodes_name_value_pattern(nodePtr->children, namePattern, valuePattern, prefix))
                return res;
        }

        if (nodePtr->next)
        {
            if (auto res = get_xmlNodes_name_value_pattern(nodePtr->next, namePattern, valuePattern, prefix))
                return res;
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
                        const char *attrName = reinterpret_cast<const char *>(attrNode->name);
                        xmlChar *value = xmlNodeListGetString(
                            nodePtr->doc,
                            attrNode->children,
                            1);

                        if (value && attr == reinterpret_cast<const char *>(attrName))
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

    bool find_xmlNode_value(xmlNodePtr nodePtr, const std::string &value, const std::string &prefix)
    {
        if (!nodePtr)
        {
            return false;
        }

        if (nodePtr->type == XML_ELEMENT_NODE && nodePtr->children != nullptr)
        {
            if (nodePtr->children->content != nullptr)
            {
                std::string contentValue = reinterpret_cast<const char *>(nodePtr->children->content);
                if (!contentValue.empty())
                {
                    if (contentValue.find(value) != std::string::npos)
                    {
                        return true;
                    }
                }
            }
        }
        if (nodePtr->children != nullptr)
        {
            if (find_xmlNode_value(nodePtr->children, value))
            {
                return true;
            }
        }

        if (nodePtr->next != nullptr)
        {
            if (find_xmlNode_value(nodePtr->next, value))
            {
                return true;
            }
        }

        return false;
    }

    std::string get_class_name(xmlNodePtr nodePtr)
    {
        if (nodePtr->type == XML_TEXT_NODE || nodePtr->name == nullptr)
            return std::string();

        std::string nodeName(reinterpret_cast<const char *>(nodePtr->name));
        return nodeName;
    }

    std::string get_attribute_value(xmlNodePtr nodePtr, const std::string &name, const std::string &prefix)
    {
        for (xmlAttr *attrNode = nodePtr->properties; attrNode; attrNode = attrNode->next)
        {

            if (prefix == "")
            {
                if (attrNode->name && name == reinterpret_cast<const char *>(attrNode->name))
                {
                    xmlChar *value = xmlNodeListGetString(nodePtr->doc, attrNode->children, 1);
                    return reinterpret_cast<const char *>(value);
                }
            }
            else
            {
                if (attrNode->name && attrNode->ns && name == reinterpret_cast<const char *>(attrNode->name) && prefix == reinterpret_cast<const char *>(attrNode->ns->prefix))
                {
                    xmlChar *value = xmlNodeListGetString(nodePtr->doc, attrNode->children, 1);
                    return reinterpret_cast<const char *>(value);
                }
            }
        }
        return std::string{};
    }
}