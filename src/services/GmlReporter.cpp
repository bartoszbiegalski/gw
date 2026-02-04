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
            auto classMap = obj.get()->GetClassNames(prefix);
            for (auto i : classMap)
            {
                std::cout << i.first << " " << i.second << "\n";
            }
        }
    }
}
