#ifndef TEXT_GUI_H
#define TEXT_GUI_H

#include <iostream>
#include <string>

#include "core/GMLObject.h"
#include "io/FileExport.h"
#include "exceptions/AllExceptions.h"
#include "services/OperationManager.h"
#include "services/Reporter.hpp"
#include "utils/string_operations.h"

int frameWidth = 69;
int taskAmount = 5;
std::string horizontalLine = "---------------------------------------------------------------------";
std::string versionLine = "|                        GML WORKER v. 1.0.0                        |";
std::string horizontalSpace = "|                                                                   |";

bool areFilesLoaded = false;
std::string noFilesLoaded = "|    No files loaded                                                |";

std::string taskTable[5] = {
    "| 1. Import file (import <file_path_1> ..)                          |",
    "| 2. Parse imported file <file_name> 								 |",
    "| 3. Divide file (divide <file_name> <ns_1> ...)                    |",
    "| 4. Merge files (merge <file_name_1> ...)                          |",
    "| 5. Print info (fino <file_name>                                   |",
    "| 6. Exit (exit)                                                    |"};

void mainLoop()
{
    OperationManager om;
    std::map<std::string, GMLObject *> loadedObjects = {};
    std::string errorLine;
    while (true)
    {
        std::cout << "\033[2J\033[1;1H"; // ANSI escape code

        std::cout << horizontalLine << std::endl;
        std::cout << versionLine << std::endl;
        std::cout << horizontalLine << std::endl;

        if (areFilesLoaded)
        {
            for (auto i : loadedObjects)
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
                    for (auto gml : i.second->getPrefixMap())
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
        std::list<std::string> lParams = string_namespace::get_schemaLocation_list(command);

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
                    GMLObject *newObject = new GMLObject(nullptr);
                    try
                    {
                        om.import(vParams[i], newObject);
                    }
                    catch (const FileNotFoundException &e)
                    {
                        errorLine = std::string("Exception: ") + e.what();
                        newObject = nullptr;
                    }
                    if (newObject != nullptr)
                    {
                        loadedObjects.insert(std::make_pair(newObject->getFileName(), newObject));
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
        /// 2. DIVIDE
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
                auto gmlObject = loadedObjects[vParams[1]];
                std::vector<std::string> vNamespaces;
                for (int i = 2; i < vParams.size(); i++)
                {
                    vNamespaces.push_back(vParams[i]);
                }
                std::string checkError = "";
                NamespaceTool::checkNamespaces(vNamespaces, gmlObject, checkError);
                if (checkError.empty() == true)
                {
                    std::vector<GMLObject *> dividedFiles;
                    om.divide(gmlObject, vNamespaces, dividedFiles);

                    FileExport fe;
                    for (auto i : dividedFiles)
                    {
                        loadedObjects.insert(std::make_pair(i->getFileName(), i));
                        fe.ExportGML(i);
                    }
                }
                else
                {
                    errorLine = checkError;
                }
            }
        }
        /// 3. MERGE
        else if (!vParams.empty() && vParams[0] == "merge")
        {
            if (vParams.size() < 3 || vParams[2].empty())
            {
                errorLine = "Need more than one file to merge!";
            }
            else
            {
                std::vector<std::string> unloadedFiles;
                for (int i = 1; i < vParams.size(); i++)
                {
                    if (loadedObjects.find(vParams[i]) == loadedObjects.end())
                    {
                        unloadedFiles.push_back(vParams[i]);
                    }
                }
                if (unloadedFiles.size() == 0)
                {
                    std::vector<GMLObject *> gmlObjects;
                    GMLObject *mergedObject = new GMLObject(nullptr);
                    for (int i = 1; i < vParams.size(); i++)
                    {
                        gmlObjects.push_back(loadedObjects[vParams[i]]);
                    }
                    om.merge(gmlObjects, mergedObject);
                    FileExport fe;
                    fe.ExportGML(mergedObject);
                    loadedObjects.insert(std::make_pair(mergedObject->getFileName(), mergedObject));

                    std::cout << "Press Enter to continue: ";
                    std::getchar();
                    /// merging
                    // om.merge()
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
        else if (!vParams.empty() && vParams[0] == "report")
        {
            Reporter::printRaport(loadedObjects[vParams[1]], 1);
            std::getchar();
        }
        /// 5. EXIT
        else if (!vParams.empty() && vParams[0] == "exit")
        {
            break;
        }
        /// Error
        else
        {
            errorLine = "Execution: <option> <param_1> ...";
        }
    }
}

#endif // TEXT_GUI_H