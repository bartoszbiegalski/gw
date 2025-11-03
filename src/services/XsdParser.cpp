#include "services/XsdParser.h"

#include <iostream>

XmlSchemaValidCtxtPtr XsdParser::getValidCtxt(const std::string &xsdPath)
{
    XmlSchemaParserCtxtPtr parserCtxt(
        xmlSchemaNewParserCtxt(xsdPath.c_str()),
        xmlSchemaFreeParserCtxt);
    if (!parserCtxt)
        throw FileNotFoundException(xsdPath);

    XmlSchemaPtr schema(
        xmlSchemaParse(parserCtxt.get()),
        xmlSchemaFree);
    if (!schema)
        throw WrongXsdFileException(xsdPath);

    XmlSchemaValidCtxtPtr validCtxt(
        xmlSchemaNewValidCtxt(schema.get()),
        xmlSchemaFreeValidCtxt);
    if (!validCtxt)
        throw XsdWrongContextException(xsdPath);

    return validCtxt;
}

void XsdParser::Parse(std::unique_ptr<XmlConfig> &cfg, std::unique_ptr<Object> &obj)
{
    auto doc = xmlReadFile(obj.get()->getFilePath().string().c_str(), nullptr, 0);
    if (!doc)
    {
        throw FileNotFoundException(obj.get()->getFilePath());
    }
    // chcemy miec liste sciezek XSD. zeby to zrobic, wezmiemy z obj mape namespace-ow i wybierzemy te, ktore maja jakies elementy w mapie elementow. potem wg. tychże sparsujemy
    auto xsdVec = NamespaceTool::GetXsdVector(obj);
    for (const auto &[prefix, fileName] : xsdVec)
    {
        auto validCtxt = getValidCtxt(fileName);
        for (const auto &[nodeId, nodePtr] : obj.get()->getGmlStorage().getGmlMap().at(prefix))
        {
            xmlDocPtr tempDoc = xmlNewDoc(BAD_CAST "1.0");
            xmlNodePtr copiedNode = xmlDocCopyNode(nodePtr.get(), tempDoc, 1);
            std::cout << copiedNode->name << "\n";
            xmlDocSetRootElement(tempDoc, copiedNode);
            std::cout << copiedNode->name << "\n";

            int result = xmlSchemaValidateDoc(validCtxt.get(), tempDoc);
            if (result != 0)
            {
                std::cout << "node niepoprawny\n";
            }
            else
            {
                std::cout << "node poprawny\n";
            }
            xmlFreeDoc(tempDoc);
        }

        // if (xmlSchemaValidateDoc(validCtxt.get(), doc) != 0)
        // {
        //     throw XsdParseException(fileName);
        // }
    }
    xmlFreeDoc(doc);

    xmlCleanupParser();
}