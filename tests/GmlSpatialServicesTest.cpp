#include <gtest/gtest.h>

#include "core/types.h"
#include "core/GmlObject.h"
#include "exceptions/Exceptions.h"
#include "services/XmlConfig.h"
#include "services/XsdConfig.h"
#include "logic/GmlServices.h"
#include "io/GmlCreate.h"
#include "io/GmlImport.h"
#include "io/GmlExport.h"
#include "utils/tree_operations.h"
#include "utils/gml_operations.h"

class GmlSpatialServicesTest : ::testing::Test
{
protected:
    FilePath testPath;
    FilePath testFile1;

    void SetUp() override
    {
        testPath = std::filesystem::path("C:/msys64/test/gw");
        testFile1 = testPath / "GmlSpatialServicesFile1.gml";
    }
};

TEST_F(GmlSpatialServicesTest, IsAreaTouching)
{
    auto sourceObj = std::make_unique<GmlObject>();
    auto destObj = std::make_unique<GmlObject>();

    auto &gml = GmlServices::Get();

    auto gmlCfg = std::make_unique<XmlConfig>(FilePath{"resources/config.json"});
    std::map<std::string, std::unique_ptr<XsdConfig>> xsdCfgs;

    xsdCfgs["egb"] = std::make_unique<XsdConfig>(FilePath{"resources/egb.json"});
    xsdCfgs["ot"] = std::make_unique<XsdConfig>(FilePath{"resources/ot.json"});

    gml.Init(
        std::move(gmlCfg),
        std::move(xsdCfgs));

    EXPECT_NO_THROW({
        gml.Get().PerformImport(testFile1, sourceObj);
    });
}