// divide.exe
// Program made for C-GEO purposes, creates exe
// Only on Windows!
// <file_name> <-z?> <ns_1> <ns_2> ..
#ifdef _WIN32
// main.cpp
#include <windows.h>
#include <shellapi.h>
#include <iostream>
#include <filesystem>
#include <vector>
#include <string>

#include "logic/GmlServices.h"

std::string wstring_to_utf8(const std::wstring &wstr)
{
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string str(size_needed - 1, 0); // -1 żeby nie kopiować null terminatora
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, str.data(), size_needed, nullptr, nullptr);
    return str;
}

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

    if (argc < 2)
    {
        std::wcerr << L"Użycie: program <ścieżka> [argumenty...]\n";
        LocalFree(wargv);
        return -1;
    }

    // if 'z' was given in wargs, create zip
    std::string zipOpt = wstring_to_utf8(wargv[1]);
    int argC{0};
    bool isZip = false;
    std::filesystem::path filePath;
    if (zipOpt == "-z" || zipOpt == "-Z")
    {
        filePath = std::filesystem::path(wargv[2]);
        argC = 3;
        isZip = true;
    }
    else
    {
        filePath = std::filesystem::path(wargv[1]);
        argC = 2;
        isZip = false;
    }
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

    std::vector<std::string> nsVect;
    for (int i = argC; i < argc; ++i)
    {
        nsVect.push_back(wstring_to_utf8(wargv[i]));
    }

    GmlServices::PerformDivision(filePath, nsVect, isZip);
    LocalFree(wargv);
    return 0;
}

#endif // _WIN32
