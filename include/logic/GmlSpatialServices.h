#pragma once

#include <geos.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/operation/valid/IsValidOp.h>

#include "core/types.h"
#include "core/GmlGeometry.h"

class GmlSpatialServices
{
public:
    static bool IsTouching(const geos::geom::CoordinateSequence &firstSequence, const geos::geom::CoordinateSequence &secondSequence);

    static GmlGeometry CreateGmlGeometryFromJO(const xmlNodePtr &node);
    static std::vector<geos::geom::CoordinateSequence> CreateCoordinateSequences(const xmlNodePtr &node, const std::string &prefix);

private:
};