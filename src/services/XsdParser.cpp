#include "services/XsdParser.h"

#include <iostream>

XmlSchemaValidCtxtPtr XsdParser::getValidCtxt(const std::string &xsdPath)
{

    XmlSchemaParserCtxtPtr parserCtxt(
        xmlSchemaNewParserCtxt(xsdPath.c_str()),
        xmlSchemaFreeParserCtxt);
    if (!parserCtxt)
        throw FileNotFoundException(xsdPath);

    XmlSchemaPtr schema(xmlSchemaParse(parserCtxt.get()), xmlSchemaFree);
    if (!schema)
        throw WrongXsdFileException(xsdPath);

    XmlSchemaValidCtxtPtr validCtxt(
        xmlSchemaNewValidCtxt(schema.get()),
        xmlSchemaFreeValidCtxt);
    if (!validCtxt)
        throw XsdWrongContextException(xsdPath);

    return validCtxt;
}

void XsdParser::Parse(std::unique_ptr<XmlConfig> &cfg, std::unique_ptr<GmlObject> &obj)
{
    std::string gmlPrefix{cfg->get("gml_prefix", "gml")};
    std::string sep{cfg->get("gml_separator", ":")};
    std::string childName{gmlPrefix + sep + cfg->get("gml_structure.child", "")};

    // chcemy miec liste sciezek XSD. zeby to zrobic, wezmiemy z obj mape namespace-ow i wybierzemy te, ktore maja jakies elementy w mapie elementow. potem wg. tychże sparsujemy
    auto xsdVec = NamespaceTool::GetXsdVector(obj);

    for (const auto &[prefix, fileName] : xsdVec)
    {
        auto fullPath = std::filesystem::current_path() / fileName;
        auto validCtxt = getValidCtxt(fullPath.string());

        for (const auto &[nodeId, nodePtr] : obj->getGmlStorage().getGmlMap().at(prefix))
        {
            // Tworzymy NOWY dokument
            auto tempDoc = NamespaceTool::CreateXmlDoc(cfg, obj);
            xmlNodePtr root = xmlDocGetRootElement(tempDoc);
            xmlNodePtr nodeChild = xmlNewChild(root, NULL, BAD_CAST childName.c_str(), NULL);
            xmlNodePtr copied = xmlDocCopyNode(nodePtr.get(), tempDoc, 1);
            xmlAddChild(nodeChild, copied);
            xmlReconciliateNs(tempDoc, root);

            xmlSaveFormatFileEnc("plik.xml", tempDoc, "UTF-8", 1);

            std::cout << "przed result\n";
            auto doc = xmlReadFile(obj->getFilePath().u8string().c_str(), nullptr, 0);

            int result = xmlSchemaValidateDoc(validCtxt.get(), tempDoc);

            if (result != 0)
            {
                throw XsdParseException(obj->getFilePath().string());
            }
            else
            {
                std::cout << "node poprawny\n";
            }

            xmlFreeDoc(tempDoc); // <-- MUSISZ zwolnić dokument!
            std::getchar();
        }
    }

    xmlCleanupParser();
}

xmlParserInputPtr XsdParser::myResolver(const std::filesystem::path url, xmlParserCtxtPtr ctxt)
{
    // std::string url(URL);

    // if (url.filename == )
    // {
    //     return xmlLoadExternalEntity((const char *)"build/gml.xsd", NULL, ctxt);
    // }
    // else if (url == "http://schemas.opengis.net/iso/19139/20070417/gmd/gmd.xsd")
    // {
    //     return xmlLoadExternalEntity((const char *)"build/gmd.xsd", NULL, ctxt);
    // }

    // // domyślnie: pozwól libxml2 ładować URL normalnie
    // return xmlLoadExternalEntity(URL, ID, ctxt);
}
