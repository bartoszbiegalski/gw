#include <iostream>
#include "services/ZipSaver.h"

void ZipSaver::SaveToZip(const std::string folderName, const std::vector<std::string> fileVector)
{
    std::string zipFileName = folderName + ".zip";
    zipFile zf = zipOpen(zipFileName.c_str(), 0);
    if (!zf)
    {
        std::cerr << "Cannot create zip file\n";
        return;
    }

    for (auto &filename : fileVector)
    {
        std::ifstream file(filename, std::ios::binary);
        if (!file)
            continue;

        zip_fileinfo zi = {};
        if (ZIP_OK != zipOpenNewFileInZip(zf, filename.c_str(), &zi,
                                          nullptr, 0, nullptr, 0, nullptr, Z_DEFLATED, Z_DEFAULT_COMPRESSION))
        {
            std::cerr << "Cannot add file: " << filename << "\n";
            continue;
        }

        std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        zipWriteInFileInZip(zf, buffer.data(), buffer.size());
        zipCloseFileInZip(zf);
    }

    zipClose(zf, nullptr);
    std::cout << "ZIP created!\n";
}
