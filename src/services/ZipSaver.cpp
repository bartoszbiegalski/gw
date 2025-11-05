#include <iostream>
#include "services/ZipSaver.h"

void ZipSaver::SaveToZip(const std::string &folderName, const std::filesystem::path &folderPath, const std::vector<std::string> &fileVector)
{
    std::filesystem::path zipPath = std::filesystem::path(folderPath) / (folderName + ".zip");
    std::string zipPathStr = zipPath.generic_u8string();
    int error = 0;
    zip_t *za = zip_open(zipPathStr.c_str(), ZIP_CREATE | ZIP_TRUNCATE, &error);
    if (!za)
    {
        char buf[1024];
        zip_error_to_str(buf, sizeof(buf), error, errno);
        std::cerr << "Cannot create zip file: " << buf << "\n";
        return;
    }

    for (const auto &fileName : fileVector)
    {
        auto savePath = std::filesystem::path(folderPath) / fileName;
        zip_source_t *zs = zip_source_file(za, savePath.string().c_str(), 0, 0);
        if (!zs)
        {
            std::cerr << "Error creating zip source for " << fileName << ": " << zip_strerror(za) << "\n";
            continue;
        }

        std::filesystem::path filePath(fileName);
        zip_int64_t idx = zip_file_add(za, filePath.filename().u8string().c_str(), zs, ZIP_FL_OVERWRITE);
        if (idx < 0)
        {
            std::cerr << "Error adding file " << fileName << ": " << zip_strerror(za) << "\n";
            zip_source_free(zs);
        }
    }

    if (zip_close(za) < 0)
    {
        std::cerr << "Error closing zip: " << zip_strerror(za) << "\n";

        int zip_err_code = 0;
        int sys_err_code = 0;
        zip_error_get(za, &zip_err_code, &sys_err_code);
        std::cerr << "libzip error code: " << zip_err_code
                  << ", system error code: " << sys_err_code << "\n";
    }
}
