#include "core/types.h"
#include "external/json.hpp"

namespace static_config
{
    nlohmann::json staticData = {
        {"gml_preambule", {{"gml_version", "1.0"}, {"gml_encoding", "UTF-8"}, {"gml_standalone", "no"}}},
        {"gml_extension", ".gml"},
        {"gml_prefix", "gml"},
        {"gml_separator", ":"},
        {"id_attribute", "gml:id"},
        {"id_value", "ID-"},
        {"gml_structure", {{"root", "FeatureCollection"}, {"child", "featureMember"}, {"required_attributes", nlohmann::json::array({"gml:id", "xmlns:gml", "xmlns:xsi", "xmlns:xlink", "xsi:schemaLocation"})}, {"optional_attributes", nlohmann::json::array({"xmlns:gco", "xmlns:gmd", "xmlns:gsr", "xmlns:gss", "xmlns:gts", "xmlns:ot", "xmlns:ges", "xmlns:egb"})}}},
        {"namespace_prefix", "xmlns"},
        {"schema_location_attribute", "xsi:schemaLocation"},
        {"xsd",
         {{"xsd_files", {{"ot", nlohmann::json::array({"BDOT500.XSD", "BDOT500-1.3.XSD"})}, {"egb", nlohmann::json::array({"EGIB.XSD"})}, {"ges", nlohmann::json::array({"GESUT.XSD"})}}},
          {"xsd_extensions", {{"ot", "-BDOT500"}, {"egb", "-EGiB"}, {"ges", "-GESUT"}}}}}};
};
