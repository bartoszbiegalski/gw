#include "io/GmlCreate.h"

void GmlCreate::Create(const std::unique_ptr<XmlConfig> &cfg, const FilePath &filePath, const FileName &fileName)
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

    std::filesystem::path savePath = filePath / fileName;
    xmlSaveFormatFileEnc(savePath.u8string().c_str(), doc, enconding.c_str(), 1);
#

    xmlFreeDoc(doc);
    xmlBufferFree(buf);
}