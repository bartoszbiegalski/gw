#include "services/ObrebParser.h"

std::string ObrebParser::GetJednostkaName(const std::string &jednostkaTeryt)
{
    std::ifstream odczyt(filePath);
    std::string line{};
    std::string jednostkaName{};
    if (!odczyt.is_open())
    {
        return jednostkaName;
    }

    while (std::getline(odczyt, line))
    {
        std::stringstream lineStream(line);
        std::vector<std::string> words;
        std::string cell;
        while (std::getline(lineStream, cell, ','))
        {
            words.push_back(cell);
        }
        std::string teryt = words[2].substr(1, words[2].size() - 7);
        if (jednostkaTeryt == teryt)
        {
            jednostkaName = words[0].substr(1, words[0].size() - 2);
            odczyt.seekg(0, std::ios::end);
        }
    }
    odczyt.close();
    return jednostkaName;
}
std::string ObrebParser::GetObrebName(const std::string &obrebTeryt)
{
    std::ifstream odczyt(filePath);
    std::string line{};
    std::string obrebName{};
    if (!odczyt.is_open())
    {
        return obrebName;
    }
    while (std::getline(odczyt, line))
    {
        std::stringstream lineStream(line);
        std::vector<std::string> words;
        std::string cell;
        while (std::getline(lineStream, cell, ','))
        {
            words.push_back(cell);
        }
        std::string teryt = words[2].substr(1, words[2].size() - 2);
        if (obrebTeryt == teryt)
        {
            obrebName = words[1].substr(1, words[1].size() - 2);
            odczyt.seekg(0, std::ios::end);
        }
    }
    odczyt.close();
    return obrebName;
}