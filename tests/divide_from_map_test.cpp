#include <gtest/gtest.h>

#include "core/types.h"
#include "core/GmlObject.h"
#include "exceptions/Exceptions.h"
#include "services/XmlConfig.h"
#include "logic/GmlServices.h"
#include "io/GmlCreate.h"
#include "io/GmlImport.h"
#include "io/GmlExport.h"

// FileImport tests

class GmlDivideFromMapFixture : public ::testing::Test
{
protected:
    std::string testFile;

    void SetUp() override
    {
        testFile = (std::filesystem::temp_directory_path() / "temp.gml").string();
    }

    void TearDown() override
    {
        if (std::filesystem::exists(testFile))
        {
            std::filesystem::remove(testFile);
        }
    }
};

TEST_F(GmlDivideFromMapFixture, ImportFileNotFoundThrows)
{
    auto sourceObj = std::make_unique<GmlObject>();
    auto destObj = std::make_unique<GmlObject>();

    auto cfg = std::make_unique<XmlConfig>(static_config::staticGmlData);

    std::map<std::string, std::vector<std::string>> classMap;
    classMap["ot"] = {"OT_Budowle", "OT_Komunikacja"};

    EXPECT_NO_THROW({
        GmlImport::Import(std::filesystem::temp_directory_path() / "source.gml", sourceObj);
        NamespaceTool::Process(cfg, sourceObj);
        XmlParser::SetContent(cfg, sourceObj);

        GmlCreate::Create(cfg, std::filesystem::temp_directory_path(), "dest.gml");
        GmlImport::Import(std::filesystem::temp_directory_path() / "dest.gml", destObj);
        GmlServices::DivideFromMap(sourceObj, destObj, classMap);

        GmlExport::Export(cfg, destObj);
    });
}

// TEST_F(GmlDivideFromMapFixture, ImportWrongExtensionThrows)
// {
//     testFile = (std::filesystem::temp_directory_path() / "temp.txt").string();
//     std::ofstream out(testFile);
//     out << "dummy";
//     out.close();

//     Object obj(nullptr);

//     EXPECT_THROW(
//         FileImport::ImportObject(testFile, obj),
//         WrongExtensionException);
// }

// TEST_F(GmlDivideFromMapFixture, ImportEmptyFileThrows)
// {
//     std::ofstream out(testFile);
//     out.close();

//     Object obj(nullptr);

//     EXPECT_THROW(
//         FileImport::ImportObject(testFile, obj),
//         EmptyFileException);
// }

// TEST_F(GmlDivideFromMapFixture, ImportCorrectFileNoThrow)
// {
//     std::ofstream out(testFile);
//     out << "<gml><tag>123</tag></gml>";
//     out.close();

//     Object obj(nullptr);

//     EXPECT_NO_THROW(
//         FileImport::ImportObject(testFile, obj));
// }
