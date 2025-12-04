/**
 * @file Reporter.cpp
 * @class Reporter
 * @brief Definitions of Reporter class.
 * @author Bartosz Biegalski
 */

#include "services/GmlReporter.h"
#include "logic/GmlServices.h"
#include <stdexcept>
#include <iostream>

void GmlReporter::printRaport(const std::unique_ptr<GmlObject> &obj)
{

    std::cout << "File name: " << obj.get()->getFileName() << std::endl;
    std::cout << "File path: " << obj.get()->getFilePath() << std::endl;

    if (obj.get()->getNamespaceMap().size() > 0)
    {
        std::cout << "Avaible namespaces: \n";
        for (auto &[prefix, map] : obj.get()->getGmlStorage().getGmlMap())
        {
            std::cout << " - " << prefix << " - " << map.size() << std::endl;
            auto classMap = GmlServices::GetClassNames(obj.get(), prefix);
            for (auto i : classMap)
            {
                std::cout << i.first << " " << i.second << "\n";
            }
        }

        // std::map<std::string, int> printMap;

        // for (const auto &j : i.second)
        // {
        //     std::string key = reinterpret_cast<const char *>(j->name);
        //     printMap[key]++;
        // }

        // if (i.second.size() > 0)
        // {
        //     std::cout << i.first << ": \n";

        //     for (const auto &j : printMap)
        //     {
        //         std::cout << i.first << ":" << j.first << " - " << j.second << std::endl;
        //     }
        //     std::cout << "\n";
        // }
    }
}
