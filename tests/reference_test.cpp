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

class ReferencesFixture : public ::testing::Test
{
protected:
    std::vector<std::string> referencesVector;

    // void SetUp() override
    // {
    //     referencesVector =
    // }

    void TearDown() override
    {
        referencesVector.clear();
    }
};

TEST_F(ReferencesFixture, ReferencesCreate)
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
    });
}

TEST_F(ReferencesFixture, ReferencesFound)
{
    auto refConfig = std::make_unique<XsdConfig>(FilePath{"resources/egb.json"});
    auto refVec = refConfig.get()->get_json("PrezentacjaGraficzna");
    std::vector<std::string> refs;
    std::vector<GmlId> foundRefs;

    auto sourceObj = std::make_unique<GmlObject>();
    auto cfg = std::make_unique<XmlConfig>(static_config::staticGmlData);
    std::map<std::string, std::vector<std::string>> classMap;
    classMap["egb"] = {"PrezentacjaGraficzna"};

    EXPECT_NO_THROW({
        for (const auto &obj : refVec)
        {
            if (obj.contains("referencesTo"))
            {
                refs = obj["referencesTo"].get<std::vector<std::string>>();
            }
        }

        GmlImport::Import(std::filesystem::temp_directory_path() / "source.gml", sourceObj);
        NamespaceTool::Process(cfg, sourceObj);
        XmlParser::SetContent(cfg, sourceObj);
        for (auto className : classMap["egb"])
        {
            auto elemMap = gml_operations::GetClassMap(sourceObj, "egb", className);
            for (auto &[first, second] : elemMap)
            {
                for (auto ref : refs)
                {
                    tree_operations::traverse_gml_id(second.get(), ref, "href", foundRefs, "egb");
                }
            }
        }
    });
}

TEST_F(ReferencesFixture, GetReferencesTo)
{
    auto sourceObj = std::make_unique<GmlObject>();

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
        gml.Get().PerformImport(std::filesystem::temp_directory_path() / "source.gml", sourceObj);

        classMap["egb"] = {"EGB_DzialkaEwidencyjna"};
        for (auto i : gml.Get().GetReferencesTo(sourceObj, classMap))
        {
        }
    });
}

TEST_F(ReferencesFixture, GetReferencesFromXsd)
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

    std::map<std::string, std::vector<std::string>> referencesFrom;

    EXPECT_NO_THROW({
        gml.Get().PerformImport(std::filesystem::temp_directory_path() / "source.gml", sourceObj);

        referencesFrom = gml.Get().GetReferencesFromConfig(sourceObj);

      
    });
}