#include <gtest/gtest.h>

#include "core/types.h"
#include "core/GmlObject.h"
#include "exceptions/Exceptions.h"
#include "services/XmlConfig.h"
#include "logic/GmlServices.h"
#include "io/GmlCreate.h"
#include "io/GmlImport.h"
#include "io/GmlExport.h"
#include "utils/gml_operations.h"

class GmlOperationsTest : public ::testing::Test
{
protected:
    FilePath testPath;
    FilePath testFile1;
    FilePath testFile2;
    FilePath testFile3;

    void SetUp() override
    {
        testPath = std::filesystem::path("C:/msys64/test/gw");
        testFile1 = testPath / "DivideFromIdVectorFile.gml";
    }

    // void TearDown() override
    // {
    //     if (std::filesystem::exists(testFile))
    //     {
    //         std::filesystem::remove(testFile);
    //     }
    // }
};

TEST_F(GmlOperationsTest, DivideFromIdVectorTest)
{
    auto sourceObj = std::make_unique<GmlObject>();
    auto destObj = std::make_unique<GmlObject>();

    auto cfg = std::make_unique<XmlConfig>(
        FilePath{"resources/config.json"});

    std::vector<GmlId> idVector = {
        "PL.PZGiK.1004.EGiB_04022323-1610-8636-2530-321713075445_2022-04-29T12-51-50",
        "PL.PZGiK.1004.EGiB_71856364-4787-1226-7186-206777823087_2022-04-29T12-51-50",
        "PL.PZGiK.1004.EGiB_52460500-8717-2513-0155-075105460422_2022-04-29T12-51-50"};

    GmlImport::Import(testFile1, sourceObj);
    NamespaceTool::Process(cfg, sourceObj);
    XmlParser::SetContent(cfg, sourceObj);

    EXPECT_NO_THROW({
        gml_operations::DivideFromIdVector(sourceObj, destObj, idVector);
        for (auto &[prefix, m] : destObj.get()->getGmlStorage().getGmlMap())
        {
            for (auto &[id, node] : m)
            {
                std::cout << id << '\n';
            }
        }
    });
}