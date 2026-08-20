#include <gtest/gtest.h>

#include <geos.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/operation/valid/IsValidOp.h>>

#include "core/types.h"
#include "io/TxtImport.h"

class TxtImportTest : public ::testing::Test
{
protected:
    FilePath testPath;
    FilePath testFile1;

    void SetUp() override
    {
        testPath = std::filesystem::path("C:/msys64/test/gw");
        testFile1 = testPath / "AreaTest.txt";
    }
};

TEST_F(TxtImportTest, ImportFile)
{
    geos::geom::CoordinateSequence sequence;
    EXPECT_NO_THROW({
        TxtImport::Import(testFile1, sequence);
        std::cout << sequence << '\n';
    });
}
