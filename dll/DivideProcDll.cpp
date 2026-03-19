// DivideProcDll.cpp

#include <windows.h>
#include <vector>
#include <string>
#include <sstream>
#include <codecvt>
#include <locale>
#include "logic/GmlServices.h"

std::string WideToUtf8(const std::wstring &wstr)
{
    if (wstr.empty())
        return {};
    int len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string out(len - 1, '\0');
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, out.data(), len, nullptr, nullptr);
    return out;
}

typedef void(__stdcall *LogCallback)(const wchar_t *msg);

static LogCallback g_log = nullptr;

extern "C"
{
    __declspec(dllexport) void __stdcall SetLogCallback(LogCallback cb)
    {
        g_log = cb;
    }

    void Log(const std::wstring &msg)
    {
        if (g_log)
            g_log(msg.c_str());
    }
}

extern "C"
{
    __declspec(dllexport) int __stdcall DivideGmlW(const wchar_t *inFile, const wchar_t *params)
    {
        if (!inFile)
            return -1;

        std::vector<std::string> nsVect;
        bool isZip = false;
        if (params)
        {
            std::wstring wstr(params);
            std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
            std::string utf8str = conv.to_bytes(wstr);
            std::istringstream iss(utf8str);
            std::string token;
            while (iss >> token)
            {
                if (token == "-z" || token == "-Z")
                {
                    isZip = true;
                }
                else
                {
                    nsVect.push_back(NamespacePrefix{token});
                }
            }
        }
        std::string inFileU8 = WideToUtf8(inFile);
        std::filesystem::path filePath(inFileU8);
        auto cfg = std::make_unique<XmlConfig>(static_config::staticGmlData);
        try
        {
            //GmlServices::PerformDivision(filePath, nsVect, isZip);
        }
        catch (...)
        {
            Log(L"exception");
        }
        return 0;
    }

    __declspec(dllexport) int __stdcall DivideGmlA(const char *inFile, void *params)
    {
        if (!inFile)
            return -1;

        std::vector<std::string> nsVect;
        bool isZip = false;
        if (params)
        {
            const char **args = static_cast<const char **>(params);
            for (int i = 0; args[i] != nullptr; ++i)
            {
                std::string argStr(args[i]);
                if (argStr == "-Z" || argStr == "-z")
                {
                    isZip = true;
                }
                else
                {
                    nsVect.push_back(NamespacePrefix{argStr});
                }
            }
        }

        std::string fileIn(inFile);
        // GmlServices::PerformDivision(fileIn, nsVect, isZip);
        return 0;
    }
}
