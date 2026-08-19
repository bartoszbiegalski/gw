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

class GmlServicesTest : public ::testing::Test
{
protected:
    std::string testFile;

    void SetUp() override
    {
        testFile = (std::filesystem::temp_directory_path() / "test.gml").string();
    }

    // void TearDown() override
    // {
    //     if (std::filesystem::exists(testFile))
    //     {
    //         std::filesystem::remove(testFile);
    //     }
    // }
};

TEST_F(GmlServicesTest, ImportFileNotFoundThrows)
{
    // auto sourceObj = std::make_unique<GmlObject>();
    // auto destObj = std::vector<std::unique_ptr<GmlObject>>();

    // auto cfg = std::make_unique<XmlConfig>(
    //     FilePath{"resources/config.json"});

    // std::map<std::string, std::vector<std::string>> classMap;

    // EXPECT_NO_THROW({
    //     GmlImport::Import(testFile, sourceObj);
    //     NamespaceTool::Process(cfg, sourceObj);
    //     XmlParser::SetContent(cfg, sourceObj);

    //     GmlDivide::Divide(cfg, sourceObj, std::vector<NamespacePrefix>{"ot"}, destObj);
    //     GmlExport::Export(cfg, destObj[0]);
    // });
}