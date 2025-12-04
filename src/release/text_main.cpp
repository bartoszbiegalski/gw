#include <iostream>
#include <string>

#include "core/GmlObject.h"
#include "exceptions/Exceptions.h"
#include "io/GmlImport.h"
#include "io/GmlExport.h"
#include "logic/GmlDivide.h"
#include "logic/GmlMerge.h"
#include "logic/GmlServices.h"
#include "services/XsdParser.h"
#include "services/GmlReporter.h"
#include "utils/string_operations.h"

int frameWidth = 69;
int taskAmount = 7;
std::string horizontalLine = "---------------------------------------------------------------------";
std::string versionLine = "|                        GML WORKER v. 1.0.0                        |";
std::string horizontalSpace = "|                                                                   |";

bool areFilesLoaded = false;
std::string noFilesLoaded = "|    No files loaded.                                               |";

std::string taskTable[7] = {
    "| 1. Import file (import <file_path_1> ..)                          |",
    "| 2. Parse imported file (parse <file_name>)                        |",
    "| 3. Divide file (divide <file_name> <ns_1> ...)                    |",
    "| 4. Merge files (merge <dest_file> <source_file_1> ...)            |",
    "| 5. Export file (export <dest_path> <file_name>                    |",
    "| 6. Print info (info <file_name>)                                  |",
    "| 7. Exit (exit)                                                    |"};

void mainLoop()
{
    auto cfg = std::make_unique<XmlConfig>(static_config::staticData);
    std::map<std::string, std::unique_ptr<GmlObject>> loadedObjects = {};
    std::string errorLine{};
    while (true)
    {
        std::cout << "\033[2J\033[1;1H"; // ANSI escape code

        std::cout << horizontalLine << std::endl;
        std::cout << versionLine << std::endl;
        std::cout << horizontalLine << std::endl;

        if (areFilesLoaded)
        {
            for (auto &i : loadedObjects)
            {
                if (i.first != " ")
                {
                    std::string stringFrame = {};
                    stringFrame = "| " + i.first;
                    for (int j = 0; j < frameWidth - i.first.size() - 3; j++)
                    {
                        stringFrame += " ";
                    }
                    stringFrame += "|";
                    std::cout << stringFrame << std::endl;
                    for (auto gml : i.second.get()->getGmlStorage().getGmlMap())
                    {
                        stringFrame = "| -> " + gml.first + ": " + std::to_string(gml.second.size());
                        int fileFrameLength = stringFrame.length();
                        for (int k = 0; k < frameWidth - fileFrameLength - 1; k++)
                        {
                            stringFrame += " ";
                        }
                        stringFrame += "|";
                        // for (int k = 0; k < frameWidth - stringFrame.length(); k++)
                        // {
                        //     stringFrame += " ";
                        // }
                        // stringFrame += "|";
                        std::cout << stringFrame << std::endl;
                    }
                }
                else
                {
                    std::cout << "Warning: null object in loadedObjects.\n";
                }
            }
        }
        else
        {
            std::cout << noFilesLoaded << std::endl;
        }
        std::cout << horizontalLine << std::endl;

        for (int i = 0; i < taskAmount; i++)
        {
            std::cout << taskTable[i] << std::endl;
            std::cout << horizontalSpace << std::endl;
        }
        std::cout << horizontalLine << std::endl;
        if (errorLine != "")
        {
            std::cout << errorLine << std::endl;
            std::cout << "\n";
        }
        errorLine = "";

        std::string command = {};
        std::cout << "> ";
        std::getline(std::cin, command);
        std::list<std::string> lParams{};
        string_operations::set_schema_location_list(command, lParams);

        std::vector<std::string> vParams;
        for (auto i : lParams)
        {
            vParams.push_back(i);
        }
        /// 1. IMPORT
        if (!vParams.empty() && vParams[0] == "import")
        {
            if (vParams.size() < 2 || vParams[1].empty())
            {
                errorLine = "Empty import path!";
            }
            else
            {
                std::vector<std::string> unloadedFiles;
                for (int i = 1; i < vParams.size(); i++)
                {
                    std::unique_ptr<GmlObject> obj = std::make_unique<GmlObject>();
                    try
                    {
                        std::filesystem::path filePath(vParams[i]);
                        GmlImport::Import(filePath.string(), obj);
                        NamespaceTool::Process(cfg, obj);
                        XmlParser::SetContent(cfg, obj);
                    }
                    catch (const FileNotFoundException &e)
                    {
                        errorLine = std::string("Exception: ") + e.what();
                        obj = nullptr;
                    }
                    if (obj != nullptr)
                    {
                        loadedObjects.emplace(std::make_pair(obj.get()->getFileName(), std::move(obj)));
                        areFilesLoaded = true;
                    }
                    else
                    {
                        unloadedFiles.push_back(vParams[i]);
                    }
                }
                if (unloadedFiles.size() > 0)
                {
                    errorLine = "Couldn't load files: ";
                    for (auto unloadedFile : unloadedFiles)
                    {
                        errorLine += unloadedFile + " ";
                    }
                }
            }
        }
        /// 2. Parse
        else if (!vParams.empty() && vParams[0] == "parse")
        {
            if (vParams.size() < 2 || vParams[1].empty())
            {
                errorLine = "Empty file name to parse!";
            }
            else if (loadedObjects.find(vParams[1]) == loadedObjects.end())
            {
                errorLine = "Wrong file name to parse!";
            }
            else
            {
                XsdParser::Parse(cfg, loadedObjects.at(vParams[1]));
            }
        }
        /// 3. DIVIDE
        else if (!vParams.empty() && vParams[0] == "divide")
        {
            if (vParams.size() < 2 || vParams[1].empty())
            {
                errorLine = "Empty file name to divide!";
            }
            else if (vParams.size() < 3)
            {
                errorLine = "No namespace given to divide!";
            }
            else if (loadedObjects.find(vParams[1]) == loadedObjects.end())
            {
                errorLine = "Wrong file name to divide!";
            }
            else
            {
                std::vector<std::string> vNamespaces;
                for (int i = 2; i < vParams.size(); i++)
                {
                    vNamespaces.push_back(vParams[i]);
                }
                auto dividedFiles = std::vector<std::unique_ptr<GmlObject>>();
                GmlDivide::Divide(cfg, loadedObjects.at(vParams[1]), vNamespaces, dividedFiles);

                for (auto &i : dividedFiles)
                {
                    loadedObjects.emplace(i->getFileName(), std::move(i));
                }
            }
        }
        // 4. MERGE
        else if (!vParams.empty() && vParams[0] == "merge")
        {
            if (vParams.size() < 3 || vParams[2].empty())
            {
                errorLine = "Usage: merge <destPath> <sourcePath_1> <sourcePath_2> <...>\n";
            }
            else
            {
                auto path = std::filesystem::path(vParams[1]);
                GmlCreate::Create(cfg, path.parent_path(), path.filename().string());
                std::unique_ptr<GmlObject> destObj = std::make_unique<GmlObject>();
                GmlImport::Import(path, destObj);

                std::vector<std::string> unloadedFiles;
                for (int i = 2; i < vParams.size(); i++)
                {
                    if (loadedObjects.find(vParams[i]) == loadedObjects.end())
                    {
                        unloadedFiles.push_back(vParams[i]);
                    }
                }
                if (unloadedFiles.size() == 0)
                {
                    for (int i = 2; i < vParams.size(); i++)
                    {
                        GmlMerge::MergeOne(cfg, loadedObjects.at(vParams[i]), destObj);
                    }

                    loadedObjects.emplace(destObj.get()->getFileName(), std::move(destObj));

                    std::cout << "Press Enter to continue: ";
                    std::getchar();
                }
                else
                {
                    errorLine = "Given files aren't loaded: ";
                    for (auto unloadedFile : unloadedFiles)
                    {
                        errorLine += unloadedFile + " ";
                    }
                }
            }
        }
        /// 5. Export
        else if (!vParams.empty() && vParams[0] == "export")
        {
            if (vParams.size() < 3 || vParams[1].empty() || vParams[2].empty())
            {
                errorLine = "Usage: export <dest_path> <file_name>\n";
            }
            else if (loadedObjects.find(vParams[2]) == loadedObjects.end())
            {
                errorLine = "Wrong file name to export!";
            }
            else
            {
                std::filesystem::path destPath = std::filesystem::path(vParams[1]) / vParams[2];
                loadedObjects.at(vParams[2]).get()->setFilePath(destPath);
                GmlExport::Export(cfg, std::move(loadedObjects.at(vParams[2])));
            }
        }
        // 6. Print info
        else if (!vParams.empty() && vParams[0] == "info")
        {
            if (loadedObjects.find(vParams[1]) == loadedObjects.end())
            {
                errorLine = "Wrong file name to show info!";
            }
            else
            {
                std::cout << "\033[2J\033[1;1H";
                GmlReporter::printRaport(loadedObjects[vParams[1]]);
                std::getchar();
            }
        }
        /// 5. EXIT
        else if (!vParams.empty() && vParams[0] == "exit")
        {
            break;
        }
        // Error
        else
        {
            errorLine = "Execution: <option> <param_1> ...";
        }
    }
}

int main()
{
    mainLoop();
}