#include "io/TxtImport.h"

void TxtImport::Import(const FilePath &filePath, geos::geom::CoordinateSequence &coordinateSequence)
{
    std::ifstream txtFile;
    std::string x, y;
    txtFile.open(filePath.c_str());
    while (txtFile >> x >> y)
    {
        double dx = std::round(stod(x) * 100.0) / 100.0;
        double dy = std::round(stod(y) * 100.0) / 100.0;
        geos::geom::Coordinate c(dx, dy);
        coordinateSequence.add(c);
    }
}