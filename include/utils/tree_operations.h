#pragma once

#include "core/types.h"

namespace tree_operations
{
    // we want to have clean functions with shardedPtr, and some traversal methods with raw pointers

    void traverse_gml_id(xmlNodePtr nodePtr, const std::string &name, const std::string &attr, std::vector<GmlId> &foundId, const std::string &prefix = "/");

    xmlNodePtr get_xmlNode_with_attr(xmlNodePtr nodePtr, const std::string &attr, const std::string &prefix = "/");

    void get_xmlNodes_with_attr(xmlNodePtr nodePtr, std::vector<xmlNodePtr> &foundXmlNodes, const std::string &attr, const std::string &prefix);
}