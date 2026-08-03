#pragma once

#include <vector>
#include <string>

#include <geos/geom/LinearRing.h>
#include <geos/geom/Geometry.h>
#include <geos/geom/Polygon.h>
#include "core/types.h"

class GmlGeometry
{
public:
    GmlGeometry() = default;

    GmlGeometry(const GmlGeometry &) = delete;
    GmlGeometry &operator=(const GmlGeometry &) = delete;

    GmlGeometry(GmlGeometry &&) noexcept = default;
    GmlGeometry &operator=(GmlGeometry &&) noexcept = default;

    explicit GmlGeometry(std::unique_ptr<geos::geom::Polygon> polygon)
        : geometry(std::move(polygon)) {}

    std::unique_ptr<geos::geom::Geometry> &GetGeometry() { return this->geometry; }

private:
    std::unique_ptr<geos::geom::Geometry> geometry;
};