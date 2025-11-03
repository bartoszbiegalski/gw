#include "logic/GmlServices.h"
#include "services/ZipSaver.h"
#include <iostream>

void GmlServices::PerformDivision(const std::string &inFile, std::vector<NamespacePrefix> &nsVec)
{
    // JSON w formie stringa
    std::string jsonText = R"(
    {
        "gml_preambule": {
            "gml_version": 1.0,
            "gml_encoding": "UTF-8",
            "gml_standalone": "no"
        },

        "gml_extension": ".gml",
        "gml_prefix": "gml",
        "gml_separator": ":",

        "id_attribute": "gml:id",
        "id_value": "ID-",

        "gml_structure": {        
            "root": "FeatureCollection",
            "child": "featureMember",       
            "required_attributes": ["gml:id", "xmlns:gml", "xmlns:xsi", "xmlns:xlink", "xsi:schemaLocation"],
            "optional_attributes": ["xmlns:gco", "xmlns:gmd", "xmlns:gsr", "xmlns:gss", "xmlns:gts", "xmlns:ot", "xmlns:ges", "xmlns:egb"]
        },

        "namespace_prefix": "xmlns",
        "schema_location_attribute": "xsi:schemaLocation",

        "xsd": {
            "xsd_files": {
                "ot": ["BDOT500.XSD", "BDOT500-1.3.XSD"],
                "egb": ["EGIB.XSD"],
                "ges": ["GESUT.XSD"]
            },
            "xsd_extensions": {
                "ot": "-BDOT500",
                "egb": "-EGiB",
                "ges": "-GESUT"
            }
        }
    })";

    // Tworzymy tymczasowy plik JSON
    std::filesystem::path tmpJsonPath = std::filesystem::temp_directory_path() / "temp_config.json";
    std::ofstream ofs(tmpJsonPath);
    if (!ofs)
    {
        return;
    }
    ofs << jsonText;
    ofs.close();

    // Tworzymy konfigurację z pliku
    auto cfg = std::make_unique<XmlConfig>(tmpJsonPath);
    std::unique_ptr<Object> obj = std::make_unique<Object>();
    GmlImport::Import(inFile, obj);
    NamespaceTool::Process(cfg, obj);
    XmlParser::SetContent(cfg, obj);
    auto objVec = std::vector<std::unique_ptr<Object>>();

    if (nsVec.size() < 1)
    {
        for (auto i : obj.get()->getGmlStorage().getGmlMap())
        {
            nsVec.push_back(i.first);
        }
    }

    GmlDivide::Divide(cfg, obj, nsVec, objVec);
    for (auto &o : objVec)
    {
        GmlExport::Export(cfg, o);
    }

    std::filesystem::remove(tmpJsonPath);
    std::vector<std::string> zipFileVec;

    // lambda
    auto fillZipVec = [&zipFileVec](const auto &objects)
    {
        for (const auto &obj : objects)
        {
            zipFileVec.push_back(obj->getFileName());
        }
    };

    fillZipVec(objVec);
    ZipSaver::SaveToZip(obj.get()->getFilePath().stem().string(), zipFileVec);
}

std::map<std::string, std::string> GmlServices::GetRootInfoMap(const Object *obj)
{
    auto rootInfoMap = std::map<std::string, std::string>();
    rootInfoMap.insert(std::make_pair("Elementy:", std::to_string(getElementAmount(obj))));
    std::string valityString = obj->getXmlValidity() == true ? "tak " : "nie";
    rootInfoMap.insert(std::make_pair("Czy poprawny XML: ", valityString));

    // returnMap.insert(std::make_pair("Czy poprawny pod wzgledem xml: ", TODO: tu jakas funkcja)); cos chcemy z ta walidacja

    return rootInfoMap;
}

std::map<std::string, std::string> GmlServices::GetNamespaceNodeInfoMap(const Object *obj, const NamespacePrefix &prefix)
{
    auto namespaceNodeMap = std::map<std::string, std::string>();
    namespaceNodeMap.insert(std::make_pair("Elementy: ", std::to_string(getNamespaceElementAmount(obj, prefix))));
    namespaceNodeMap.insert(std::make_pair("Plik XSD: ", getNamespaceDataFromPrefix(obj, prefix).NamespaceUrl));
    std::string valityString = obj->getGmlStorage().getXsdValidity(prefix) == true ? "tak " : "nie";
    namespaceNodeMap.insert(std::make_pair("Czy poprawny XSD: ", valityString));

    return namespaceNodeMap;
}

int GmlServices::getElementAmount(const Object *obj)
{
    return std::accumulate(obj->getGmlStorage().getGmlMap().begin(),
                           obj->getGmlStorage().getGmlMap().end(), 0,
                           [&](int acc, const auto &pair)
                           {
                               return acc + pair.second.size();
                           });
}

int GmlServices::getNamespaceElementAmount(const Object *obj, const NamespacePrefix &prefix)
{
    if (obj->getGmlStorage().getGmlMap().find(prefix) == obj->getGmlStorage().getGmlMap().end())
        return 0;
    else
        return obj->getGmlStorage().getGmlMap().at(prefix).size();
}

NamespaceData GmlServices::getNamespaceDataFromPrefix(const Object *obj, const NamespacePrefix &prefix)
{
    if (obj->getNamespaceMap().find(prefix) == obj->getNamespaceMap().end())
        return NamespaceData{};
    else
        return obj->getNamespaceMap().at(prefix);
}

std::map<std::string, int> GmlServices::GetClassNames(const Object *obj, const NamespacePrefix &prefix)
{
    std::map<std::string, int> classNameMap;

    const auto &gmlMap = obj->getGmlStorage().getGmlMap();
    auto found = gmlMap.find(prefix);
    if (found == gmlMap.end())
        return classNameMap;

    const auto &entries = found->second;

    for (const auto &[nsPrefix, vector] : entries)
    {
        for (auto it = vector.get(); it != nullptr; it = it->next)
        {
            if (it->type == XML_TEXT_NODE || it->name == nullptr)
                continue;

            std::string nodeName(reinterpret_cast<const char *>(it->name));

            classNameMap[nodeName]++;
        }
    }

    return classNameMap;
}

std::vector<Entry> GmlServices::GetClassVector(const Object *obj, const NamespacePrefix &prefix, const std::string className)
{
    auto entryVector = std::vector<Entry>();
    if (obj->getGmlStorage().getGmlMap().find(prefix) == obj->getGmlStorage().getGmlMap().end())
    {
        return entryVector;
    }
    else
    {
        for (auto &[prefix, vector] : obj->getGmlStorage().getGmlMap().at(prefix))
        {
            auto it = vector.get();
            while (it != nullptr && it->type != XML_TEXT_NODE)
            {
                if (reinterpret_cast<const char *>(it->name) == className)
                {
                    entryVector.emplace_back(prefix, vector);
                }
                it = it->next;
            }
        }
    }
    return entryVector;
}
