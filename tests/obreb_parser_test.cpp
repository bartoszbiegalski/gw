#include <gtest/gtest.h>

#include "core/types.h"
#include "services/ObrebParser.h"

class ObrebParserFixture : public ::testing::Test
{
protected:
    std::string testFile1;

    void SetUp() override
    {
        testFile1 = (std::filesystem::temp_directory_path() / "obreb_parser_testfile1.txt").string();
    }
};

TEST_F(ObrebParserFixture, ObrebParserImport)
{
    std::string r1, r2;
    std::string obrebTeryt = "020101_1.0015";
    std::string jednostkaTeryt = "020101_1";
    ObrebParser op = ObrebParser(std::filesystem::path(testFile1));

    r1 = op.GetJednostkaName(jednostkaTeryt);
    r2 = op.GetObrebName(obrebTeryt);

    EXPECT_TRUE(!r1.empty() && !r2.empty());
}