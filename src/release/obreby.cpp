#ifdef _WIN32
// main.cpp
#include <windows.h>
#include <shellapi.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <string>

#include "io/GmlImport.h"
#include "io/GmlExport.h"
#include "logic/GmlDivide.h"
#include "logic/GmlMerge.h"
#include "logic/GmlServices.h"

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    int argc;
    wchar_t **wargv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (!wargv)
    {
        std::cerr << "Nie udało się pobrać argumentów\n";
        return -1;
    }

    if (argc < 3)
    {
        std::cerr << "Użycie: obreby <-flaga> <plik.gml>\n";
        LocalFree(wargv);
        return -1;
    }
    std::wstring ws(wargv[1]);
    std::string flag(ws.begin(), ws.end());
    if (flag != "-o" && flag != "-j")
    {
        std::cerr << "Nieznana opcja: użyj -o / -j";
        LocalFree(wargv);
        return -1;
    }
    std::filesystem::path filePath = std::filesystem::path(wargv[2]);

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if (!std::filesystem::exists(filePath))
    {
        std::wstring msg = L"Plik nie istnieje: " + filePath.wstring() + L"\n";
        DWORD written;
        WriteConsoleW(hConsole, msg.c_str(), (DWORD)msg.size(), &written, nullptr);
        LocalFree(wargv);
        return -1;
    }

    std::wstring wstr = L"Ścieżka: " + filePath.wstring() + L"\n";
    DWORD written;
    auto gmlCfg = std::make_unique<XmlConfig>(static_config::staticGmlData);
    std::map<std::string, std::unique_ptr<XsdConfig>> xsdCfgs;

    auto &gml = GmlServices::Get();

    gml.Init(
        std::move(gmlCfg),
        std::move(xsdCfgs)
    );
    if (flag == "-o")
    {
        gml.Get().PerformDivisionO(filePath, true);
    }
    else
    {
        gml.Get().PerformDivisionJ(filePath, true);
    }
    // gml.Get()::


    LocalFree(wargv);
    return 0;
}

#endif