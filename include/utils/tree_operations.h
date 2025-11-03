#pragma once

#include "core/types.h"

namespace tree_operations
{
    // we want to have clean functions with shardedPtr, and some traversal methods with raw pointers

    GmlNodePtr &get_gml_node(const GmlNodePtr &gmlNodePtr, const std::string name);

    xmlNodePtr get_xmlNode_with_attr(xmlNodePtr nodePtr, const std::string &attr, const std::string &prefix = "/");

    // GmlNodePtr &get_gml_node(const GmlNodePtr &gmlNodePtr, const std::list<std::string, std::string> attrList);
}