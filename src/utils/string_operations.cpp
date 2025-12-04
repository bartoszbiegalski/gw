#include "utils/string_operations.h"

namespace string_operations
{
    std::string get_prefix(const std::string &key, const std::string &sep)
    {
        auto pos = key.find(sep);
        if (pos == std::string::npos)
            return ""; // brak delimitera - zwracamy pusty string lub cały string - zależnie od potrzeb
        return key.substr(0, pos);
    }

    std::string get_suffix(const std::string &key, const std::string &sep)
    {
        auto pos = key.find(sep);
        if (pos == std::string::npos || pos + 1 >= key.size())
            return ""; // brak delimitera lub brak sufiksu
        return key.substr(pos + 1);
    }
    int levenstein_distance(const std::string &s1, const std::string &s2)
    {
        size_t m = s1.size();
        size_t n = s2.size();
        std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));

        for (size_t i = 0; i <= m; i++)
            dp[i][0] = i;
        for (size_t j = 0; j <= n; j++)
            dp[0][j] = j;

        for (size_t i = 1; i <= m; i++)
        {
            for (size_t j = 1; j <= n; j++)
            {
                int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
                dp[i][j] = std::min({dp[i - 1][j] + 1,
                                     dp[i][j - 1] + 1,
                                     dp[i - 1][j - 1] + cost});
            }
        }
        return dp[m][n];
    }

    bool validate_schemaLocation_stream(const std::string &schemaLocationStream)
    {
        std::string str = schemaLocationStream;

        std::istringstream iss(str);
        std::string attributeValue;
        std::list<std::string> schemaLocationList;

        while (iss >> attributeValue)
        {
            schemaLocationList.push_back(attributeValue);
        }

        if (schemaLocationList.size() % 2 != 0)
        {
            return false;
        }

        return true;
    }

    void set_schema_location_list(const std::string &schemaLocationStream, std::list<std::string> &schemaLocationList)
    {
        std::string str = schemaLocationStream;

        std::istringstream iss(str);
        std::string attributeValue;

        while (iss >> attributeValue)
        {
            schemaLocationList.push_back(attributeValue);
        }
    }

    void set_schema_location_string(const std::list<std::string> &schemaLocationList, std::string &schemaLocationStream)
    {
        for (const auto &it : schemaLocationList)
        {
            schemaLocationStream += it + " ";
        }
    }
}
