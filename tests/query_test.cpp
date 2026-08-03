#include <gtest/gtest.h>

#include "core/types.h"
#include "core/GmlObject.h"
#include "exceptions/Exceptions.h"
#include "services/XmlConfig.h"
#include "services/XsdConfig.h"
#include "services/GmlReporter.h"
#include "logic/GmlServices.h"
#include "io/GmlCreate.h"
#include "io/GmlImport.h"
#include "io/GmlExport.h"
#include "utils/tree_operations.h"
#include "utils/gml_operations.h"

// Query request tests

class QueryFixture : public ::testing::Test
{
protected:
    std::string testFile1;
    std::string testFile2;
    std::string testFile3;
    std::string testFile4;
    std::string testFile5;

    void SetUp() override
    {
        testFile1 = (std::filesystem::temp_directory_path() / "aSzkoleniowy2021.gml").string();
        testFile2 = (std::filesystem::temp_directory_path() / "egib1.gml").string();
        testFile3 = (std::filesystem::temp_directory_path() / "egib2.gml").string();
        testFile4 = (std::filesystem::temp_directory_path() / "egib3.gml").string();
        testFile5 = (std::filesystem::temp_directory_path() / "egib4.gml").string();
    }
};

// TEST_F(QueryFixture, QueryCreate)
// {
//     auto cfg = std::make_unique<XmlConfig>(FilePath{"resources/config.json"});
//     auto query = cfg.get()->get_json("gml_queries.find_within_object.egb-obreb");
// }
TEST_F(QueryFixture, QueryFind)
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

    std::vector<GmlId> reszta;

    EXPECT_NO_THROW({
        gml.Get().PerformImport(testFile5, sourceObj);
        auto tree = gml.Get().GetJO(sourceObj, "EGB_ObrebEwidencyjny", reszta);
        // gml.PerformDivisionO(testFile1);
        // gml.Get().PerformDivisionJ(testFile4, true);
        // gml.Get().PerformDivisionO(testFile4, true);
        // GmlReporter::printReport(sourceObj);

        // auto tree1 = gml.Get().GetJO(sourceObj, "EGB_JednostkaEwidencyjna");
        // auto tree2 = gml.Get().GetJO(sourceObj, "EGB_ObrebEwidencyjny");

        //     auto queryElements = gml.Get().GetElementsFromQuery(sourceObj, "gml_filters.filter_by_class", "EGB_ObrebEwidencyjny");

        //     gml.Get().PerformCreateGml(std::filesystem::temp_directory_path() / "egib1-dest.gml");
        //     gml.Get().PerformImport(std::filesystem::temp_directory_path() / "egib1-dest.gml", destObj);
        //     auto idSet = queryElements["egb"];
        //     std::vector<GmlId> idVector(idSet.begin(), idSet.end());
        //     GmlDivide::DivideFromIdVector(gml.Get().gmlCfg, sourceObj, "egb", idVector, destObj);

        //     gml.Get().PerformExport(destObj);
    });
}
