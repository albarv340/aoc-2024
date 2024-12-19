#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>
#include <unordered_map>

std::unordered_map<std::string, long long> poss;

std::vector<std::string> allowed;
long long possibilities(std::string cols)
{
    long long amount{0};
    if (poss.count(cols))
    {
        return poss[cols];
    }
    bool possible = false;
    if (cols.size() == 0)
    {
        return 1;
    }
    for (auto const &a : allowed)
    {
        if (cols.find(a) == 0)
        {
            std::string substr{cols.substr(a.size(), cols.size())};
            long long substr_poss = possibilities(substr);
            poss[substr] = substr_poss;
            amount += substr_poss;
        }
    }
    return amount;
}

int main()
{
    std::ifstream file("input.txt");
    if (!file)
    {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    std::vector<std::string> colors;

    std::string line;
    std::regex rgx("\\s*,\\s*");
    if (std::getline(file, line))
    {
        std::sregex_token_iterator iter(line.begin(), line.end(), rgx, -1);
        std::sregex_token_iterator end;
        for (; iter != end; ++iter) {
            allowed.push_back(*iter);
        }
    }

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;
        colors.push_back(line);
    }
    file.close();

    long long sum{0};
    for (const std::string &col : colors)
    {
        long long amount = possibilities(col);
        sum += amount;
        poss[col] = amount;
    }
    std::cout << sum << std::endl;

    return 0;
}
