#include "exceptions/Exceptions.h"
#include "io/GmlImport.h"

#include <fstream>
#include <iostream>

void GmlImport::Import(const FilePath &filePath, std::unique_ptr<Object> &obj)
{
    if (!std::filesystem::exists(filePath) || !std::filesystem::is_regular_file(filePath))
    {
        throw FileNotFoundException(filePath.filename().u8string());
    }

    if (filePath.extension() != ".gml")
    {
        throw WrongExtensionException(filePath.filename().u8string());
    }

    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open())
    {
        throw FileNotFoundException(filePath.filename().u8string());
    }
    file.seekg(0, std::ios::end);
    if (file.tellg() == 0)
    {
        throw EmptyFileException(filePath.filename().u8string());
    }
    file.seekg(0, std::ios::beg);

    obj->setFileName(filePath.filename().u8string());
    obj->setFilePath(filePath.u8string());
}