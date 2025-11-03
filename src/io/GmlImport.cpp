#include "exceptions/Exceptions.h"
#include "io/GmlImport.h"

#include <fstream>
#include <iostream>

void GmlImport::Import(const FilePath &filePath, std::unique_ptr<Object> &obj)
{
    if (!std::filesystem::exists(filePath) || !std::filesystem::is_regular_file(filePath))
    {
        throw FileNotFoundException(filePath.filename().string());
    }

    if (filePath.extension() != ".gml")
    {
        throw WrongExtensionException(filePath.filename().string());
    }

    std::ifstream file(filePath);
    if (!file.is_open())
    {
        throw FileNotFoundException(filePath.filename().string());
    }
    if (file.peek() == std::ifstream::traits_type::eof())
    {
        throw EmptyFileException(filePath.filename().string());
    }
    obj->setFileName(filePath.filename().string());
    obj->setFilePath(filePath.string());
}