#pragma once

#include <geos.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/operation/valid/IsValidOp.h>

#include "core/types.h"

class TxtImport
{
public:
    TxtImport() = delete;

    static void Import(const FilePath &filePath, geos::geom::CoordinateSequence &coordinateSequence);
};