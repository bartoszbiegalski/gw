#pragma once

#include "core/types.h"

namespace tree_operations
{
    // we want to have clean functions with shardedPtr, and some traversal methods with raw pointers

    void traverse_gml_id(xmlNodePtr nodePtr, const std::string &name, const std::string &attr, std::vector<GmlId> &foundId, const std::string &prefix = "/");

    // return node with given name
    xmlNodePtr get_xmlNode_from_name(xmlNodePtr nodePtr, const std::string &name, const std::string &prefix = "/");

    xmlNodePtr get_xmlNodes_name_value_pattern(xmlNodePtr nodePtr, const std::string &namePattern, const std::string &valuePattern, const std::string &prefix = "/");

    xmlNodePtr get_xmlNode_with_attr(xmlNodePtr nodePtr, const std::string &attr, const std::string &prefix = "/");

    void get_xmlNodes_with_names(xmlNodePtr nodePtr, std::vector<xmlNodePtr> &foundNodes, const std::vector<std::string> &names, const std::string &prefix);

    void get_xmlNodes_with_attr(xmlNodePtr nodePtr, std::vector<xmlNodePtr> &foundXmlNodes, const std::string &attr, const std::string &prefix);

    bool find_xmlNode_value(xmlNodePtr nodePtr, const std::string &value, const std::string &prefix = "/");

    std::string get_class_name(xmlNodePtr nodePtr);


}