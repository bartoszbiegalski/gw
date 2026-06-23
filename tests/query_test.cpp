
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

// Query request tests

class QueryFixture : public ::testing::Test
{
protected:
    std::string testFile;

    void SetUp() override
    {
        testFile = (std::filesystem::temp_directory_path() / "egib1.gml").string();
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

    std::string query_type = "gml_filters";
    std::string query_request = "filter_by_class";
    auto classMap = gmlCfg.get()->get_json(query_type + '.' + query_request);

    gml.Init(
        std::move(gmlCfg),
        std::move(xsdCfgs));

    std::map<ClassName, std::map<NamespacePrefix, std::vector<std::pair<GmlId, int>>>> queryMap;

    // EXPECT_NO_THROW({
    //     gml.Get().PerformImport(testFile, sourceObj);
    //     auto queryElements = gml.Get().GetElementsFromQuery(sourceObj, "gml_filters.filter_by_class", "EGB_ObrebEwidencyjny");

    //     gml.Get().PerformCreateGml(std::filesystem::temp_directory_path() / "egib1-dest.gml");
    //     gml.Get().PerformImport(std::filesystem::temp_directory_path() / "egib1-dest.gml", destObj);
    //     auto idSet = queryElements["egb"];
    //     std::vector<GmlId> idVector(idSet.begin(), idSet.end());
    //     GmlDivide::DivideFromIdVector(gml.Get().gmlCfg, sourceObj, "egb", idVector, destObj);

    //     gml.Get().PerformExport(destObj);
    // });
}