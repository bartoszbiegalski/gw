
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
    std::map<std::string, std::vector<GmlId>> query_elements;

    void TearDown() override
    {
        query_elements.clear();
    }
};

TEST_F(QueryFixture, QueryCreate)
{
    auto cfg = std::make_unique<XmlConfig>(FilePath{"resources/config.json"});
    auto query = cfg.get()->get_json("gml_queries.find_within_object.egb-obreb");
}
TEST_F(QueryFixture, QueryFind)
{ 
    auto cfg = std::make_unique<XmlConfig>(FilePath{"resources/config.json"});
    std::string query_type = "find_within_object";
    std::string query_request = "egb-obreb";
    auto query = cfg.get()->get_json("gml_queries." + query_type + '.' + query_request);
    auto classMap = query["classes"];
    std::cout<<classMap.is_array()<<"\n";
    EXPECT_NO_THROW({
        if (query_request == "egb-obreb")
        {
            for (auto i : classMap)
            {
                std::cout<<i["className"].get<std::string>()<<std::endl;
                std::cout<<i["idLabel"].get<std::string>()<<std::endl;
                std::cout<<i["elementName"].get<std::string>()<<std::endl;
            }
        }
    });
}