// DivideProcDll.cpp

#ifdef _WIN32
#include <windows.h>
#include <vector>
#include <string>
#include "logic/GmlServices.h"

extern "C"
{
    /// UTF-16
    typedef void(_stdcall *TProgressW)(const wchar_t *message, void *user);
    /// ANSI
    typedef void(_stdcall *TProgressA)(const char *message, void *user);

    __declspec(dllexport) int __stdcall DivideGmlW(
        const wchar_t *inFile,
        TProgressW progress,
        void *userData)
    {
        if (!inFile)
            return -1;

        std::vector<std::string> nsVect;

        if (userData)
        {
            const wchar_t **args = static_cast<const wchar_t **>(userData);
            for (int i = 0; args[i] != nullptr; ++i)
            {
                std::wstring ws(args[i]);
                nsVect.push_back(NamespacePrefix{std::string(ws.begin(), ws.end())});
            }
        }

        std::wstring wsIn(inFile);
        std::string fileIn(wsIn.begin(), wsIn.end());

        GmlServices::PerformDivision(fileIn, nsVect, false);
        return 0;
    }

    __declspec(dllexport) int __stdcall DivideGmlA(
        const char *inFile,
        TProgressA progress,
        void *userData)
    {
        if (!inFile)
            return -1;

        std::vector<std::string> nsVect;

        if (userData)
        {
            const char **args = static_cast<const char **>(userData);
            for (int i = 0; args[i] != nullptr; ++i)
            {
                nsVect.push_back(NamespacePrefix{std::string(args[i])});
            }
        }

        std::string fileIn(inFile);
        GmlServices::PerformDivision(fileIn, nsVect, false);
        return 0;
    }
}

#endif