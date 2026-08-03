#include "exceptions/Exceptions.h"
#include "io/GmlImport.h"

#include <fstream>
#include <iostream>

void GmlImport::Import(const FilePath &filePath, std::unique_ptr<GmlObject> &obj)
{
    if (!std::filesystem::exists(filePath) || !std::filesystem::is_regular_file(filePath))
    {
        throw FileNotFoundException(filePath.filename().string());
    }

    if (filePath.extension() != ".gml")
    {
        throw WrongExtensionException(filePath.filename().string());
    }

    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open())
    {
        throw FileNotFoundException(filePath.filename().string());
    }
    file.seekg(0, std::ios::end);
    if (file.tellg() == 0)
    {
        throw EmptyFileException(filePath.filename().string());
    }
    file.seekg(0, std::ios::beg);

    obj->setFileName(filePath.filename().string());
    obj->setFilePath(filePath.string());
}