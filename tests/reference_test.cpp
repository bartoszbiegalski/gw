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

// FileImport tests

class FindReferencesTest : public ::testing::Test
{
protected:
    FilePath testPath;
    FilePath testFile1;

    void SetUp() override
    {
        testPath = std::filesystem::path("C:/msys64/test/gw");
        testFile1 = testPath / "FindReferencesTest.gml";
    }
};
TEST_F(FindReferencesTest, ReferencesCreate)
{
    EXPECT_NO_THROW({
        auto cfg = std::make_unique<XsdConfig>(FilePath{"resources/egb.json"});
        auto refVec = cfg.get()->get_json("EGB_JednostkaRejestrowaBudynkow");
        std::vector<std::string> refs;

        for (const auto &obj : refVec)
        {
            if (obj.contains("referencesTo"))
            {
                refs = obj["referencesTo"].get<std::vector<std::string>>();
            }
        }
        for (auto r : refs)
        {
            std::cout << r << '\n';
        }
    });
}

TEST_F(FindReferencesTest, FindReferences)
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

    std::vector<GmlId> refs{"EGB_DzialkaEwidencyjn.9n"};

    EXPECT_NO_THROW({
        gml.Get().PerformImport(testFile1, sourceObj);
        auto foundReferences = gml.Get().GetReferencesTo(sourceObj, refs);
    });
}

/*
TEST_F(ReferencesFixture, GetReferencesTo)
{
    auto sourceObject = std::make_unique<GmlObject>();

    auto &gml = GmlServices::Get();

    auto gmlCfg = std::make_unique<XmlConfig>(FilePath{"resources/config.json"});
    std::map<std::string, std::unique_ptr<XsdConfig>> xsdCfgs;

    xsdCfgs["egb"] = std::make_unique<XsdConfig>(FilePath{"resources/egb.json"});
    xsdCfgs["ot"] = std::make_unique<XsdConfig>(FilePath{"resources/ot.json"});

    gml.Init(
        std::move(gmlCfg),
        std::move(xsdCfgs));

    std::map<std::string, std::vector<std::string>> classMap;

    EXPECT_NO_THROW({
        gml.Get().PerformImport(std::filesystem::temp_directory_path() / "source.gml", sourceObject);

        classMap["egb"] = {"EGB_DzialkaEwidencyjna"};
        for (auto &[prefix, refMap] : gml.Get().GetReferencesTo(sourceObject, classMap))
        {
            std::cout << prefix << '\n';
            for (auto r : refMap)
            {
                std::cout << r.first << " " << r.second.size() << "\n";
            }
        }
    });
}

TEST_F(ReferencesFixture, GetReferencesFromXsd)
{
    auto sourceObject = std::make_unique<GmlObject>();
    auto destObj = std::make_unique<GmlObject>();

    auto &gml = GmlServices::Get();

    auto gmlCfg = std::make_unique<XmlConfig>(FilePath{"resources/config.json"});
    std::map<std::string, std::unique_ptr<XsdConfig>> xsdCfgs;

    xsdCfgs["egb"] = std::make_unique<XsdConfig>(FilePath{"resources/egb.json"});
    xsdCfgs["ot"] = std::make_unique<XsdConfig>(FilePath{"resources/ot.json"});

    gml.Init(
        std::move(gmlCfg),
        std::move(xsdCfgs));

    std::map<std::string, std::vector<std::string>> referencesFrom;

    EXPECT_NO_THROW({
        gml.Get().PerformImport(std::filesystem::temp_directory_path() / "source.gml", sourceObject);

        referencesFrom = gml.Get().GetReferencesFromConfig(sourceObject);
    });
}
*/