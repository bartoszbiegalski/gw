#include "logic/GmlSpatialServices.h"
#include "utils/tree_operations.h"

bool GmlSpatialServices::IsIntersecting(const GmlNodePtr &first, const GmlNodePtr &second)
{
    geos::geom::CoordinateXY c1;
    //  std::string firstPosList = tree_operations::find_xmlNode_value(first.get(), "")

    return false;
}

GmlGeometry GmlSpatialServices::CreateGmlGeometryFromJO(const xmlNodePtr &node)
{
    GmlGeometry gmlGeometry;
    std::string polygonLabel = "Polygon";
    auto polygonNode = tree_operations::get_xmlNode_from_name(node, polygonLabel, "gml");
    if (!polygonNode->children)
    {
        return gmlGeometry;
    }

    auto factory = geos::geom::GeometryFactory::create();
    polygonNode = polygonNode->children->next;

    std::unique_ptr<geos::geom::LinearRing> shell;
    std::vector<std::unique_ptr<geos::geom::LinearRing>> holes;

    while (polygonNode != nullptr)
    {
        std::string int_ext_string = std::string(reinterpret_cast<const char *>(polygonNode->name)); // exterior / interior
        auto positionNode = polygonNode->children->next;                                             // linearRing
        if (!positionNode)
        {
            return gmlGeometry;
        }

        positionNode = positionNode->children->next;
        if (!positionNode)
        {
            return gmlGeometry;
        }

        geos::geom::CoordinateSequence seq;
        for (auto newNode = positionNode; newNode != nullptr; newNode = newNode->next)
        {
            if (!newNode->children || !newNode->children->content)
                continue;

            std::string pos = std::string(reinterpret_cast<const char *>(newNode->children->content));
            std::stringstream ss(pos);
            std::string x, y;
            while (ss >> x >> y)
            {
                double dx = std::round(stod(x) * 100.0) / 100.0;
                double dy = std::round(stod(y) * 100.0) / 100.0;
                geos::geom::Coordinate c(dx, dy);
                seq.add(c);
            }
        }
        if (std::string(reinterpret_cast<const char *>(polygonNode->name)) == "exterior")
        {
            shell = factory.get()->createLinearRing(seq);
        }

        else if (std::string(reinterpret_cast<const char *>(polygonNode->name)) == "interior")
        {
            auto hole = factory.get()->createLinearRing(seq);
            holes.push_back(std::move(hole));
        }
        polygonNode = polygonNode->next->next;
    }
    std::unique_ptr<geos::geom::Polygon> polygon = factory->createPolygon(std::move(shell), std::move(holes));
    gmlGeometry = GmlGeometry(std::move(polygon));

    return gmlGeometry;
}

std::vector<geos::geom::CoordinateSequence> GmlSpatialServices::CreateCoordinateSequences(const xmlNodePtr &node, const std::string &prefix)
{
    std::vector<geos::geom::CoordinateSequence> sequences;
    std::string geometryLabel = "geometria";
    auto geometryNode = tree_operations::get_xmlNode_from_name(node, geometryLabel, prefix);
    if (!geometryNode)
    {
        return sequences;
    }

    std::vector<std::string> posNames = {"pos", "posList"};
    std::vector<xmlNodePtr> foundNodes;
    tree_operations::get_xmlNodes_with_names(geometryNode, foundNodes, posNames, "gml");
    for (auto node : foundNodes)
    {
        geos::geom::CoordinateSequence seq;
        std::string pos = std::string(reinterpret_cast<const char *>(node->children->content));
        std::stringstream ss(pos);
        std::string x, y;
        while (ss >> x >> y)
        {
            double dx = std::round(stod(x) * 100.0) / 100.0;
            double dy = std::round(stod(y) * 100.0) / 100.0;
            geos::geom::Coordinate c(dx, dy);
            seq.add(c);
        }
        sequences.push_back(seq);
    }
    return sequences;
}