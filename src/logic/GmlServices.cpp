#include "logic/GmlServices.h"
#include "services/ZipSaver.h"
#include <iostream>

GmlServices &GmlServices::Get()
{
    static GmlServices instance;
    return instance;
}

void GmlServices::Init(std::unique_ptr<XmlConfig> gmlCfg, std::map<std::string, std::unique_ptr<XsdConfig>> xsdCfgs)
{
    if (!initialized)
    {
        this->gmlCfg = std::move(gmlCfg);
        this->xsdCfgs = std::move(xsdCfgs);
        initialized = true;
    }
}
void GmlServices::PerformCreateGml(const std::filesystem::path &filePath)
{
    GmlCreate::Create(gmlCfg, filePath);
}

void GmlServices::PerformImport(const std::filesystem::path &inFile, std::unique_ptr<GmlObject> &importedObject)
{
    importedObject = std::make_unique<GmlObject>();
    try
    {
        GmlImport::Import(inFile, importedObject);
        NamespaceTool::Process(gmlCfg, importedObject);
        XmlParser::SetContent(gmlCfg, importedObject);
    }
    catch (const BaseException &e)
    {
        throw e;
    }
}
void GmlServices::PerformExport(const std::unique_ptr<GmlObject> &exportObject)
{
    GmlExport::Export(gmlCfg, exportObject);
}

void GmlServices::PerformDivision(const std::filesystem::path &inFile, std::vector<NamespacePrefix> &nsVec, bool isZip)
{
    std::unique_ptr<GmlObject> obj = std::make_unique<GmlObject>();
    GmlImport::Import(inFile, obj);
    NamespaceTool::Process(gmlCfg, obj);
    XmlParser::SetContent(gmlCfg, obj);
    auto objVec = std::vector<std::unique_ptr<GmlObject>>();

    if (nsVec.size() < 1)
    {
        for (auto i : obj.get()->getGmlStorage().getGmlMap())
        {
            nsVec.push_back(i.first);
        }
    }
    GmlDivide::Divide(gmlCfg, obj, nsVec, objVec);
    for (auto &o : objVec)
    {
        GmlExport::Export(gmlCfg, o);
    }

    if (isZip)
    {
        std::vector<std::string> zipFileVec;
        auto fillZipVec = [&zipFileVec](const auto &objects)
        {
            for (const auto &obj : objects)
            {
                zipFileVec.push_back(obj->getFileName());
            }
        };
        fillZipVec(objVec);
        ZipSaver::SaveToZip(obj.get()->getFilePath().stem().u8string(), obj.get()->getFilePath().parent_path(), zipFileVec);

        for (const auto &obj : objVec)
        {
            std::filesystem::remove(obj->getFilePath());
        }
    }
}


void GmlServices::PerformMerge(const FilePath &inFile, std::vector<FilePath> &filePathVec)
{
    GmlCreate::Create(gmlCfg, inFile.parent_path(), inFile.filename().string());
    std::unique_ptr<GmlObject> destObj = std::make_unique<GmlObject>();
    GmlImport::Import(inFile, destObj);

    for (auto sourceFilePath : filePathVec)
    {
        auto newObj = std::make_unique<GmlObject>();

        GmlImport::Import(sourceFilePath, newObj);
        NamespaceTool::Process(gmlCfg, newObj);
        XmlParser::SetContent(gmlCfg, newObj);

        GmlMerge::MergeOne(gmlCfg, newObj, destObj);
    }

    GmlExport::Export(gmlCfg, destObj);
}

std::map<std::string, std::map<GmlId, std::vector<GmlId>>> GmlServices::GetReferencesTo(const std::unique_ptr<GmlObject> &sourceObject, const std::map<std::string, std::vector<std::string>> &classMap)
{
    std::string referenceAttr = gmlCfg.get()->get("reference_attr", "href");
    auto referenceMap = std::map<std::string, std::map<GmlId, std::vector<GmlId>>>();
    for (auto &[prefix, classVector] : classMap)
    {
        for (auto &className : classVector)
        {
            auto refVec = xsdCfgs[prefix].get()->get_json(className);
            std::vector<std::string> refs;
            for (const auto &obj : refVec)
            {
                if (obj.contains("referencesTo"))
                {
                    refs = obj["referencesTo"].get<std::vector<std::string>>();
                }
            }
            auto gmlMap = gml_operations::GetClassMap(sourceObject, prefix, className);

            for (auto &[id, ptr] : gmlMap)
            {
                auto idVector = std::vector<GmlId>();
                for (auto referenceElement : refs)
                {
                    tree_operations::traverse_gml_id(ptr.get(), referenceElement, referenceAttr, idVector, prefix);
                }
                referenceMap[prefix][id] = idVector;
            }
        }
    }
    return referenceMap;
}

std::map<std::string, std::vector<GmlId>> GmlServices::GetReferencesFrom(const std::unique_ptr<GmlObject> &sourceObject, std::map<std::string, std::map<GmlId, std::vector<GmlId>>> &referencesMap)
{
    auto referencesFromMap = std::map<std::string, std::vector<GmlId>>();

    for (auto &[prefix, refMap] : referencesMap)
    {
        auto gmlMap = sourceObject.get()->getGmlStorage().getGmlMap()[prefix];
        for (auto &[gmlId, gmlIdVec] : refMap)
        {
            for (auto &refGmlID : gmlIdVec)
            {
                if (gmlMap.find(refGmlID) != gmlMap.end())
                {
                    referencesFromMap[prefix].push_back(gmlId);
                }
            }
        }
    }
    return referencesFromMap;
}

std::map<NamespacePrefix, std::vector<GmlId>> GmlServices::GetReferencesFromConfig(const std::unique_ptr<GmlObject> &sourceObject)
{
    auto referencesFromMap = gmlCfg.get()->get_json("xsd.references_from").get<std::map<std::string, std::vector<std::string>>>();
    return referencesFromMap;
}

std::map<NamespacePrefix, std::vector<GmlId>> GmlServices::GetElementsFromClass(const std::unique_ptr<GmlObject> &sourceObject, const NamespacePrefix &prefix, const ClassName &className)
{
    std::map<NamespacePrefix, std::vector<GmlId>> classIdMap;
    for (auto &[gmlId, gmlPtr] : sourceObject.get()->getGmlStorage().getGmlMap()[prefix])
    {
        auto ptr = tree_operations::get_xmlNode_from_name(gmlPtr.get(), className, prefix);
        if (ptr != nullptr)
        {
            classIdMap[prefix].push_back(gmlId);
        }
    }
    return classIdMap;
}

std::map<NamespacePrefix, std::vector<GmlId>> GmlServices::GetElementsFromClasses(const std::unique_ptr<GmlObject> &sourceObject, const std::map<NamespacePrefix, std::vector<ClassName>> &classes)
{
    std::map<NamespacePrefix, std::vector<GmlId>> classIdMap;
    for (auto &[prefix, classVec] : classes)
    {
        for (auto &[gmlId, gmlPtr] : sourceObject.get()->getGmlStorage().getGmlMap()[prefix])
        {
            for (auto className : classVec)
            {
                auto ptr = tree_operations::get_xmlNode_from_name(gmlPtr.get(), className, prefix);
                if (ptr != nullptr)
                {
                    classIdMap[prefix].push_back(gmlId);
                }
            }
        }
    }
    return classIdMap;
}

std::map<NamespacePrefix, std::set<GmlId>> GmlServices::GetElementsFromQuery(const std::unique_ptr<GmlObject> &sourceObject, const std::string &query_type, const std::string &query_request)
{
    std::map<NamespacePrefix, std::set<GmlId>> query_elements;
    auto classMap = gmlCfg.get()->get_json(query_type + '.' + query_request);

    std::string className = query_request;

    std::string idLabel = classMap.at("idLabel").get<std::string>();
    std::string elementName = classMap.at("elementName").get<std::string>();
    std::string prefix = classMap.at("namespace").get<std::string>();

    auto idMap = GetElementsFromClass(sourceObject, prefix, className);
    for (auto &[pref, mp] : idMap)
    {
        for (auto elementId : mp)
        {
            auto idNode = tree_operations::get_xmlNode_from_name(sourceObject.get()->getGmlStorage().getGmlMap()[prefix][elementId].get(), idLabel, prefix);
            auto elementNameNode = tree_operations::get_xmlNode_from_name(sourceObject.get()->getGmlStorage().getGmlMap()[prefix][elementId].get(), elementName, prefix);
            if (idNode != nullptr && elementNameNode != nullptr)
            {
                std::string idLabelValue = reinterpret_cast<const char *>(idNode->children->content);
                std::string elementNameValue = reinterpret_cast<const char *>(elementNameNode->children->content);
                std::cout<<elementId<<" "<<idLabelValue<<" "<<elementNameValue<<"\n";

                // iterate through all elements from egb and check, if any has element with name == elementNameNode->name and id == idNode->name

                std::vector<xmlNodePtr> foundXmlNodes;
                for (auto &[elId, elNode] : sourceObject.get()->getGmlStorage().getGmlMap()[prefix])
                {
                    if (tree_operations::check_xmlNodes_name_value_pattern(elNode.get(), "id", idLabelValue, prefix) == true)
                    {
                        foundXmlNodes.push_back(elNode.get());
                    }
                }
                for (auto foundNode : foundXmlNodes)
                {
                    std::string id = reinterpret_cast<const char *>(foundNode->properties->children->content);
                    if (!id.empty())
                    {
                        query_elements[prefix].insert(id);
                    }
                }
            }
        }
    }
    return query_elements;
}