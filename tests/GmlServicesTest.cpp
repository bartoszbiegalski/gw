#include <gtest/gtest.h>

#include "core/types.h"
#include "core/GmlObject.h"
#include "exceptions/Exceptions.h"
#include "services/XmlConfig.h"
#include "logic/GmlServices.h"
#include "io/TxtImport.h"
#include "io/GmlCreate.h"
#include "io/GmlImport.h"
#include "io/GmlExport.h"
#include "utils/gml_operations.h"

class GmlServicesTest : public ::testing::Test
{
protected:
    FilePath testPath;
    FilePath testFile1;
    FilePath testFile2;
    FilePath testFile3;
    FilePath testFile4;
    FilePath testFile5;

    void SetUp() override
    {
        testPath = std::filesystem::path("C:/msys64/test/gw");
        testFile1 = testPath / "GetTouchingObjectsFile.gml";
        testFile2 = testPath / "GetTouchingObjectsFile.txt";
        testFile3 = testPath / "GetReferencesToTest.gml";

        testFile4 = testPath / "IsInArea.gml";
        testFile5 = testPath / "IsInArea.txt";
    }
};

TEST_F(GmlServicesTest, GetReferencesTo)
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

    std::vector<GmlId> refs{"obiekt_id.cd9f4301-56e1-485e-aa84-d70b1344e589"};

    EXPECT_NO_THROW({
        // gml.Get().PerformImport(testFile3, sourceObj);
        // auto foundReferences = gml.Get().GetReferencesTo(sourceObj, refs);
        // std::cout << "referencji do " << refs[0] << ": " << foundReferences.size() << '\n';
    });
}

TEST_F(GmlServicesTest, GetTouchingObjects)
{
    auto sourceObj = std::make_unique<GmlObject>();
    auto destObj = std::make_unique<GmlObject>();

    geos::geom::CoordinateSequence sequence;

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
        TxtImport::Import(testFile2, sequence);

        // auto vec = gml.Get().GetTouchingElements(sourceObj, sequence);
        // std::cout << vec.size() << "\n";
    });
}

TEST_F(GmlServicesTest, IsInArea)
{
    auto sourceObj = std::make_unique<GmlObject>();
    auto destObj = std::make_unique<GmlObject>();

    geos::geom::CoordinateSequence sequence;

    auto &gml = GmlServices::Get();

    auto gmlCfg = std::make_unique<XmlConfig>(FilePath{"resources/config.json"});
    std::map<std::string, std::unique_ptr<XsdConfig>> xsdCfgs;

    xsdCfgs["egb"] = std::make_unique<XsdConfig>(FilePath{"resources/egb.json"});
    xsdCfgs["ot"] = std::make_unique<XsdConfig>(FilePath{"resources/ot.json"});

    GmlId id = "PL.PZGiK.4687.GESUT_293934a7-4eca-4095-8075-cf11cc6f1ae0_2023-09-11T12-18-26";

    gml.Init(
        std::move(gmlCfg),
        std::move(xsdCfgs));

    EXPECT_NO_THROW({
        gml.Get().PerformImport(testFile4, sourceObj);
        TxtImport::Import(testFile5, sequence);

        auto vec = gml.Get().GetTouchingElements(sourceObj, sequence);
        GmlDivide::DivideFromIdVector(gml.Get().GetGmlCfg(), sourceObj, "", vec, destObj);

        std::cout << "ile obiektow: " << vec.size() << '\n';
        if (std::find(vec.begin(), vec.end(), id) != vec.end())
        {
            std::cout << "znaleziono id: " << id << "\n";
        }
        else
        {
            std::cout << "Nie znaleziono id: " << id << "\n";
        }
    });
}