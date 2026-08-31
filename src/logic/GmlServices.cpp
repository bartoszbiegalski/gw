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

void GmlServices::PerformDivision(const FilePath &inFile, std::vector<NamespacePrefix> &nsVec, bool isZip)
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
    GmlDivide::DivideFromPrefixes(gmlCfg, obj, nsVec, objVec);
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
        // ZipSaver::SaveToZip(obj.get()->getFilePath().stem().string(), obj.get()->getFilePath().parent_path(), zipFileVec);

        for (const auto &obj : objVec)
        {
            std::filesystem::remove(obj->getFilePath());
        }
    }
}
void GmlServices::PerformDivisionJ(const FilePath &inFile, bool isRaport)
{
    std::ofstream zapis;
    std::string zapisFileName;
    FilePath zapisPath;

    std::unique_ptr<GmlObject> obj = std::make_unique<GmlObject>();
    GmlImport::Import(inFile, obj);
    NamespaceTool::Process(gmlCfg, obj);
    XmlParser::SetContent(gmlCfg, obj);

    if (isRaport)
    {
        std::string zapisFileName = obj->getFilePath().stem().string() + "-j-raport.txt";
        FilePath zapisPath = (obj->getFilePath().parent_path() / zapisFileName).u8string();
        zapis.open(zapisPath);
        zapis << " " << obj->getFileName() << ": " << std::endl;
        for (auto &[prefix, _] : obj.get()->getGmlStorage().getGmlMap())
        {
            zapis << prefix << ":\n";
            auto classMap = obj.get()->GetClassNames(prefix);
            for (auto &[className, classCount] : classMap)
            {
                zapis << "  " << className << ": " << classCount << "\n";
            }
            zapis << "\n";
        }
        zapis << "\n";
    }

    std::vector<GmlId> reszta;

    auto tree = GetJO(obj, "EGB_JednostkaEwidencyjna", reszta);
    auto objVec = std::vector<std::unique_ptr<GmlObject>>();

    GmlDivide::DivideJO(gmlCfg, tree, obj, objVec);
    for (auto &o : objVec)
    {
        GmlExport::Export(gmlCfg, o);
        if (isRaport)
        {
            zapis << o.get()->getFileName() << ": " << std::endl;
            for (auto &[prefix, _] : o.get()->getGmlStorage().getGmlMap())
            {
                zapis << prefix << ":\n";
                auto classMap = o.get()->GetClassNames(prefix);
                for (auto &[className, classCount] : classMap)
                {
                    zapis << "  " << className << ": " << classCount << "\n";
                }
                zapis << "\n";
            }
        }
    }
    std::unique_ptr<GmlObject> resztaObj = std::make_unique<GmlObject>();

    GmlDivide::DivideFromIdVector(gmlCfg, obj, "-reszta", reszta, resztaObj);
    GmlExport::Export(gmlCfg, resztaObj);
    if (isRaport)
    {
        zapis << resztaObj.get()->getFileName() << ": " << std::endl;
        for (auto &[prefix, _] : resztaObj.get()->getGmlStorage().getGmlMap())
        {
            zapis << prefix << ":\n";
            auto classMap = resztaObj.get()->GetClassNames(prefix);
            for (auto &[className, classCount] : classMap)
            {
                zapis << "  " << className << ": " << classCount << "\n";
            }
            zapis << "\n";
        }
    }
    if (isRaport)
    {
        zapis.close();
    }
}

void GmlServices::PerformDivisionO(const FilePath &inFile, bool isRaport)
{
    std::ofstream zapis;
    std::string zapisFileName;
    FilePath zapisPath;

    std::unique_ptr<GmlObject> obj = std::make_unique<GmlObject>();
    GmlImport::Import(inFile, obj);
    NamespaceTool::Process(gmlCfg, obj);
    XmlParser::SetContent(gmlCfg, obj);

    if (isRaport)
    {
        std::string zapisFileName = obj->getFilePath().stem().string() + "-o-raport.txt";
        FilePath zapisPath = (obj->getFilePath().parent_path() / zapisFileName).u8string();
        zapis.open(zapisPath);
        zapis << " " << obj->getFileName() << ": " << std::endl;
        for (auto &[prefix, _] : obj.get()->getGmlStorage().getGmlMap())
        {
            zapis << prefix << ":\n";
            auto classMap = obj.get()->GetClassNames(prefix);
            for (auto &[className, classCount] : classMap)
            {
                zapis << "  " << className << ": " << classCount << "\n";
            }
            zapis << "\n";
        }
        zapis << "\n";
    }

    std::vector<GmlId> reszta;

    auto tree = GetJO(obj, "EGB_ObrebEwidencyjny", reszta);
    auto objVec = std::vector<std::unique_ptr<GmlObject>>();

    GmlDivide::DivideJO(gmlCfg, tree, obj, objVec);
    for (auto &o : objVec)
    {
        GmlExport::Export(gmlCfg, o);
        if (isRaport)
        {
            zapis << o.get()->getFileName() << ": " << std::endl;
            for (auto &[prefix, _] : o.get()->getGmlStorage().getGmlMap())
            {
                zapis << prefix << ":\n";
                auto classMap = o.get()->GetClassNames(prefix);
                for (auto &[className, classCount] : classMap)
                {
                    zapis << "  " << className << ": " << classCount << "\n";
                }
                zapis << "\n";
            }
        }
    }
    std::unique_ptr<GmlObject> resztaObj = std::make_unique<GmlObject>();

    GmlDivide::DivideFromIdVector(gmlCfg, obj, "-reszta", reszta, resztaObj);
    GmlExport::Export(gmlCfg, resztaObj);
    if (isRaport)
    {
        zapis << resztaObj.get()->getFileName() << ": " << std::endl;
        for (auto &[prefix, _] : resztaObj.get()->getGmlStorage().getGmlMap())
        {
            zapis << prefix << ":\n";
            auto classMap = resztaObj.get()->GetClassNames(prefix);
            for (auto &[className, classCount] : classMap)
            {
                zapis << "  " << className << ": " << classCount << "\n";
            }
            zapis << "\n";
        }
    }
    if (isRaport)
    {
        zapis.close();
    }
}

void GmlServices::PerformDivisionWithIdVector(const FilePath &inFile, const std::string &fileSuffix, std::vector<GmlId> &idVector, bool isZip)
{
    std::unique_ptr<GmlObject> obj = std::make_unique<GmlObject>();
    GmlImport::Import(inFile, obj);
    NamespaceTool::Process(gmlCfg, obj);
    XmlParser::SetContent(gmlCfg, obj);
    auto destObject = std::make_unique<GmlObject>();

    GmlDivide::DivideFromIdVector(gmlCfg, obj, fileSuffix, idVector, destObject);
    GmlExport::Export(gmlCfg, destObject);
    if (isZip)
    {
        std::vector<std::string> zipFileVec;
        zipFileVec.push_back(destObject->getFileName());
        // ZipSaver::SaveToZip(destObject.get()->getFilePath().stem().string(), destObject.get()->getFilePath().parent_path(), zipFileVec);

        std::filesystem::remove(destObject->getFilePath());
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

std::map<NamespacePrefix, std::map<GmlId, std::vector<GmlId>>> GmlServices::GetReferencesTo(const std::unique_ptr<GmlObject> &sourceObject, const std::vector<GmlId> &vecId)
{
    std::string referenceAttr = gmlCfg.get()->get("reference_attr", "href");
    auto referenceMap = std::map<std::string, std::map<GmlId, std::vector<GmlId>>>();
    for (auto &[prefix, idMap] : sourceObject.get()->getGmlStorage().getGmlMap())
    {
        if (xsdCfgs.find(prefix) != xsdCfgs.end())
        {
            for (auto id : vecId)
            {
                if (idMap.find(id) != idMap.end())
                {
                    ClassName className = tree_operations::get_class_name(idMap[id].get());
                    auto refVec = xsdCfgs[prefix].get()->get_json(className);
                    std::vector<std::string> refs;
                    for (const auto &obj : refVec)
                    {
                        if (obj.contains("referencesTo"))
                        {
                            refs = obj["referencesTo"].get<std::vector<std::string>>();
                        }
                    }

                    auto idVector = std::vector<GmlId>();
                    for (auto referenceElement : refs)
                    {
                        tree_operations::traverse_gml_id(idMap[id].get(), referenceElement, referenceAttr, idVector, prefix);
                    }
                    referenceMap[prefix][id] = idVector;
                }
            }
        }
    }
    return referenceMap;
}

std::map<NamespacePrefix, std::map<GmlId, std::vector<GmlId>>> GmlServices::GetReferencesTo(const std::unique_ptr<GmlObject> &sourceObject, const std::map<NamespacePrefix, std::vector<ClassName>> &classMap)
{
    std::string referenceAttr = gmlCfg.get()->get("reference_attr", "href");
    auto referenceMap = std::map<std::string, std::map<GmlId, std::vector<GmlId>>>();
    for (auto &[prefix, classVector] : classMap)
    {
        if (xsdCfgs.find(prefix) != xsdCfgs.end())
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
    }
    return referenceMap;
}

std::map<NamespacePrefix, std::vector<GmlId>> GmlServices::GetReferencesFrom(const std::unique_ptr<GmlObject> &sourceObject, std::map<NamespacePrefix, std::map<GmlId, std::vector<GmlId>>> &referencesMap)
{
    auto referencesFromMap = std::map<NamespacePrefix, std::vector<GmlId>>();

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

std::map<NamespacePrefix, std::vector<ClassName>> GmlServices::GetReferencesFromConfig(const std::unique_ptr<GmlObject> &sourceObject)
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

GmlTreeModel GmlServices::GetJO(const std::unique_ptr<GmlObject> &sourceObject, const std::string &type, std::vector<GmlId> &reszta)
{
    GmlTreeModel obrebyTreeModel;

    std::string mainClassName;
    std::string idLabel;
    std::string idRegex;

    std::string idJednostkaRegex = R"(.*(\d{6}_[0-9]).*)";
    std::string idObrebRegex = R"(.*(\d{6}_[0-9]\.[0-9]{4}).*)";

    if (type == "EGB_JednostkaEwidencyjna")
    {
        mainClassName = type;
        idLabel = "idJednostkiEwid";
        idRegex = idJednostkaRegex;
    }
    else if (type == "EGB_ObrebEwidencyjny")
    {
        mainClassName = type;
        idLabel = "idObrebu";
        idRegex = idObrebRegex;
    }
    else
    {
        return obrebyTreeModel;
    }

    std::string elementName = "nazwaWlasna";
    std::string EGB_prefix = "egb";

    std::vector<GmlId> obreby;
    std::map<GmlId, std::string> teryty;
    std::map<GmlId, std::string> nazwy;
    std::map<GmlId, GmlGeometry> geometrie;
    std::vector<std::pair<GmlId, GmlNodePtr>> punktyGraniczne{};
    std::vector<std::pair<GmlId, GmlNodePtr>> refs{};
    std::map<std::string, std::vector<std::pair<GmlId, GmlNodePtr>>> prezentacjeGraficzne{};
    std::map<std::string, std::vector<std::pair<GmlId, GmlNodePtr>>> inneObiekty{};
    std::vector<std::string> idLabelVector = {"idJednostkiEwid", "idUzytku", "idKonturu", "idObrebu", "idDzialki", "idBudynku", "idLokalu", "idPunktu", "idJednostkiRejestrowej"};
    for (auto &[prefix, prefixMap] : sourceObject.get()->getGmlStorage().getGmlMap())
    {
        for (auto &[elId, elNode] : prefixMap)
        {
            if (prefix == EGB_prefix)
            {
                std::string className = reinterpret_cast<const char *>(elNode.get()->name);
                if (className == mainClassName)
                {
                    auto terytNode = tree_operations::get_xmlNode_from_name(elNode.get(), idLabel, EGB_prefix);
                    auto nameNode = tree_operations::get_xmlNode_from_name(elNode.get(), elementName, EGB_prefix);

                    if (terytNode != nullptr && terytNode->children != nullptr)
                    {
                        std::string teryt = reinterpret_cast<const char *>(terytNode->children->content);
                        std::string name = reinterpret_cast<const char *>(nameNode->children->content);
                        obreby.push_back(elId);
                        teryty[elId] = teryt;
                        nazwy[teryt] = name;
                        obrebyTreeModel[teryt][className].push_back(std::make_pair(elNode.get(), elId));

                        auto gmlGeometry = GmlSpatialServices::CreateGmlGeometryFromJO(elNode.get());
                        geometrie[elId] = std::move(gmlGeometry);
                    }
                }
                else if (className == "EGB_PunktGraniczny")
                {
                    punktyGraniczne.push_back(std::make_pair(elId, elNode));
                }
                else if (className == "PrezentacjaGraficzna")
                {
                    prezentacjeGraficzne[EGB_prefix].push_back(std::make_pair(elId, elNode));
                }
                else if (className == "EGB_ObiektTrwaleZwiazanyZBudynkiem" || className == "EGB_BlokBudynku")
                {
                    refs.push_back(std::make_pair(elId, elNode));
                }
                else
                {
                    inneObiekty[EGB_prefix].push_back(std::make_pair(elId, elNode));
                }
            }
            else // ot lub ges
            {
                std::string className = reinterpret_cast<const char *>(elNode.get()->name);
                if (className == "PrezentacjaGraficzna")
                {
                    prezentacjeGraficzne[prefix].push_back(std::make_pair(elId, elNode));
                }
                else
                {
                    inneObiekty[prefix].push_back(std::make_pair(elId, elNode));
                }
            }
        }
    }
    for (auto &[elId, elNode] : punktyGraniczne)
    {
        auto idNode = tree_operations::get_xmlNode_from_name(elNode.get(), "idPunktu", "egb");
        std::string className = reinterpret_cast<const char *>(elNode.get()->name);
        if (idNode != nullptr && idNode->children != nullptr)
        {
            std::string pointId = reinterpret_cast<const char *>(idNode->children->content);
            // znajdz geometrie
            if ((mainClassName == "EGB_ObrebEwidencyjny" && pointId[0] == 'D') || (mainClassName == "EGB_JednostkaEwidencyjna" && (pointId[0] == 'D' || pointId[0] == 'O' || pointId[0] == 'J')))
            {
                std::smatch m;
                if (std::regex_search(pointId, m, std::regex(idRegex)))
                {
                    std::string extracted = m.str(1);
                    obrebyTreeModel[extracted][className].push_back(std::make_pair(elNode.get(), elId));
                }
                else
                {
                    reszta.push_back(elId);
                }
            }
            else
            {
                bool isMarked = false;
                auto posNode = tree_operations::get_xmlNode_from_name(elNode.get(), "pos", "gml");
                std::string pointPosition = reinterpret_cast<const char *>(posNode->children->content);
                for (auto obrebId : obreby)
                {
                    auto obrebNode = tree_operations::get_xmlNode_from_name(sourceObject.get()->getGmlStorage().getGmlMap()[EGB_prefix][obrebId].get(), "geometria", EGB_prefix);
                    if (obrebNode != nullptr)
                    {
                        if (tree_operations::find_xmlNode_value(obrebNode, pointPosition, "gml"))
                        {
                            obrebyTreeModel[teryty[obrebId]][className].push_back(std::make_pair(elNode.get(), elId));
                            isMarked = true;
                        }
                    }
                }
                if (!isMarked)
                {
                    reszta.push_back(elId);
                }
            }
        }
        else
        {
            bool isMarked = false;
            auto posNode = tree_operations::get_xmlNode_from_name(elNode.get(), "pos", "gml");
            std::string pointPosition = reinterpret_cast<const char *>(posNode->children->content);
            for (auto obrebId : obreby)
            {
                auto obrebNode = tree_operations::get_xmlNode_from_name(sourceObject.get()->getGmlStorage().getGmlMap()[EGB_prefix][obrebId].get(), "geometria", EGB_prefix);
                if (obrebNode != nullptr)
                {
                    if (tree_operations::find_xmlNode_value(obrebNode, pointPosition, "gml"))
                    {
                        obrebyTreeModel[teryty[obrebId]][className].push_back(std::make_pair(elNode.get(), elId));
                        isMarked = true;
                    }
                }
            }
            if (!isMarked)
            {
                reszta.push_back(elId);
            }
        }
    }

    for (auto &[prefix, elVec] : inneObiekty)
    {
        for (auto &[elId, elNode] : elVec)
        {
            std::string className = reinterpret_cast<const char *>(elNode.get()->name);
            if (prefix == EGB_prefix)
            {
                auto foundNode = tree_operations::get_xmlNodes_name_value_pattern(elNode.get(), "id", idRegex, prefix);
                auto foundNodeJednostka = tree_operations::get_xmlNodes_name_value_pattern(elNode.get(), "id", idJednostkaRegex, prefix);

                if (foundNode != nullptr && foundNode->children != nullptr)
                {
                    std::string idLabel = reinterpret_cast<const char *>(foundNode->name);
                    std::string teryt = reinterpret_cast<const char *>(foundNode->children->content);
                    std::smatch m;
                    if (std::find(idLabelVector.begin(), idLabelVector.end(), idLabel) != idLabelVector.end())
                    {
                        if (std::regex_search(teryt, m, std::regex(idRegex)))
                        {
                            std::string extracted = m.str(1); // tylko wycięty kawałek
                            obrebyTreeModel[extracted][className].push_back(std::make_pair(elNode.get(), elId));
                        }
                        else
                        {
                            reszta.push_back(elId);
                        }
                    }
                    else
                    {
                        for (auto &[key, classMap] : obrebyTreeModel)
                        {
                            classMap[className].push_back(std::make_pair(elNode.get(), elId));
                        }
                    }
                }
                else if (foundNodeJednostka != nullptr && foundNodeJednostka->children != nullptr)
                {
                    std::string teryt = reinterpret_cast<const char *>(foundNodeJednostka->children->content);
                    for (auto &[key, classMap] : obrebyTreeModel)
                    {
                        auto it = classMap.find(teryt);
                        if (it != classMap.end())
                        {
                            classMap[className].push_back(std::make_pair(elNode.get(), elId));
                        }
                    }
                }
                else
                {
                    for (auto &[key, classMap] : obrebyTreeModel)
                    {
                        classMap[className].push_back(std::make_pair(elNode.get(), elId));
                    }
                }
            }
            else // ot lub ges
            {
                auto sequences = GmlSpatialServices::CreateCoordinateSequences(elNode.get(), prefix);
                auto factory = geos::geom::GeometryFactory::create();
                for (auto &[terytId, terytName] : teryty)
                {
                    auto geometry = geometrie[terytId].GetGeometry().get();
                    auto poly = dynamic_cast<const geos::geom::Polygon *>(geometry);

                    bool br = false;
                    for (auto seq : sequences)
                    {
                        for (size_t i = 0; i < seq.size(); i++)
                        {
                            std::unique_ptr<geos::geom::Point> point = factory->createPoint(seq.getAt(i));

                            if (poly->covers(point.get()))
                            {
                                // dodaj
                                obrebyTreeModel[terytName][className].push_back(std::make_pair(elNode.get(), elId));
                                br = true;
                                break;
                            }
                        }
                        if (br)
                        {
                            break;
                        }
                    }
                }
            }
        }
    }
    for (auto &[prefix, elVec] : prezentacjeGraficzne)
    {
        for (auto &[elId, elNode] : elVec)
        {
            std::vector<GmlId> foundRefs;

            std::string className = reinterpret_cast<const char *>(elNode.get()->name);
            tree_operations::traverse_gml_id(elNode.get(), "obiektPrzedstawiany", "href", foundRefs, prefix);

            for (auto foundRef : foundRefs)
            {
                for (auto &[key, classMap] : obrebyTreeModel)
                {
                    for (auto &[className, classVec] : classMap)
                    {
                        for (auto &[node, gmlId] : classVec)
                            if (gmlId == foundRef)
                            {
                                obrebyTreeModel[key]["PrezentacjaGraficzna"].push_back(std::make_pair(elNode.get(), elId));
                            }
                    }
                }
            }
        }
    }
    for (auto &[elId, elNode] : refs)
    {
        std::string refId = "";
        auto node1 = tree_operations::get_xmlNode_from_name(elNode.get(), "budynekZElementamiZwiazanymi", "egb");
        auto node2 = tree_operations::get_xmlNode_from_name(elNode.get(), "budynekZBlokiemBud", "egb");
        if (node1)
        {
            if (xmlGetProp(node1, BAD_CAST "href") != nullptr)
            {
                refId = std::string(reinterpret_cast<char *>(xmlGetProp(node1, BAD_CAST "href")));
            }
        }
        else if (node2)
        {

            if (xmlGetProp(node2, BAD_CAST "href") != nullptr)
            {
                refId = std::string(reinterpret_cast<char *>(xmlGetProp(node2, BAD_CAST "href")));
            }
        }
        if (!refId.empty())
        {
            for (auto &[key, classMap] : obrebyTreeModel)
            {
                for (auto &[className, classVec] : classMap)
                {
                    for (auto &[node, gmlId] : classVec)
                        if (gmlId == refId)
                        {
                            std::string className = std::string(reinterpret_cast<const char *>(elNode.get()->name));
                            obrebyTreeModel[key][className].push_back(std::make_pair(elNode.get(), elId));
                        }
                }
            }
        }
    }

    return obrebyTreeModel;
}

std::vector<GmlId> GmlServices::GetTouchingElements(const std::unique_ptr<GmlObject> &sourceObject, const geos::geom::CoordinateSequence sequence)
{
    std::vector<GmlId> ids;
    for (auto &[prefix, prefixMap] : sourceObject.get()->getGmlStorage().getGmlMap())
    {
        for (auto &[gmlId, nodePtr] : prefixMap)
        {
            auto sequences = GmlSpatialServices::CreateCoordinateSequences(nodePtr.get(), prefix);
            bool br = false;

            for (auto seq : sequences)
            {
                if (GmlSpatialServices::IsTouching(sequence, seq))
                {
                    ids.push_back(gmlId);
                    break;
                }
            }
        }
    }
    return ids;
}

std::vector<GmlId> GmlServices::GetIntersectingElements(const std::unique_ptr<GmlObject> &sourceObject, const geos::geom::Polygon &polygon)
{
    // TODO: Do it properly
}

std::map<GmlId, std::set<GmlId>> GmlServices::GetElementsFromQuery(const std::unique_ptr<GmlObject> &sourceObject, const std::string &query_type, const std::string &query_request, std::map<GmlId, std::pair<std::string, std::string>> &extra_attributes)
{
    std::map<GmlId, std::set<GmlId>> query_elements;
    // auto classMap = gmlCfg.get()->get_json(query_type + '.' + query_request);

    // std::string className = query_request;
    // std::string idLabel;
    // std::string elementName;
    // std::string prefix;
    // if (className == "EGB_JednostkaEwidencyjna")
    // {
    //     idLabel = "idJednostkiEwid";
    //     elementName = "nazwaWlasna";
    //     prefix = "egb";
    // }
    // else if (className == "EGB_ObrebEwidencyjny")
    // {
    //     idLabel = "idObrebu";
    //     elementName = "nazwaWlasna";
    //     prefix = "egb";
    // }
    // auto idMap = GetElementsFromClass(sourceObject, prefix, className);
    // for (auto &[pref, mp] : idMap)
    // {
    //     for (auto elementId : mp)
    //     {
    //         auto idNode = tree_operations::get_xmlNode_from_name(sourceObject.get()->getGmlStorage().getGmlMap()[prefix][elementId].get(), idLabel, prefix);
    //         auto elementNameNode = tree_operations::get_xmlNode_from_name(sourceObject.get()->getGmlStorage().getGmlMap()[prefix][elementId].get(), elementName, prefix);
    //         if (idNode != nullptr && elementNameNode != nullptr)
    //         {
    //             GmlId idLabelValue = reinterpret_cast<const char *>(idNode->children->content);
    //             std::string elementNameValue = reinterpret_cast<const char *>(elementNameNode->children->content);
    //             // iterate through all elements from egb and check, if any has element with name == elementNameNode->name and id == idNode->name

    //             std::vector<xmlNodePtr> foundXmlNodes;
    //             for (auto &[elId, elNode] : sourceObject.get()->getGmlStorage().getGmlMap()[prefix])
    //             {
    //                 if (tree_operations::check_xmlNodes_name_value_pattern(elNode.get(), "id", idLabelValue, prefix) == true)
    //                 {
    //                     foundXmlNodes.push_back(elNode.get());
    //                 }
    //             }
    //             for (auto foundNode : foundXmlNodes)
    //             {
    //                 std::string id = reinterpret_cast<const char *>(foundNode->properties->children->content);
    //                 if (!id.empty())
    //                 {
    //                     query_elements[elementId].insert(id);
    //                 }
    //             }
    //             auto pair = std::pair<std::string, std::string>(idLabelValue, elementNameValue);
    //             extra_attributes[elementId] = pair;
    //         }
    //     }
    // }
    return query_elements;
}