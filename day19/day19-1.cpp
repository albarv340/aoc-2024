#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>
#include <unordered_map>

std::unordered_map<std::string, bool> poss;

std::vector<std::string> allowed;
bool is_possible(std::string cols)
{
    if (poss.count(cols))
    {
        return poss[cols];
    }
    bool possible = false;
    if (cols.size() == 0)
    {
        return true;
    }
    bool found_match = false;
    for (auto const &a : allowed)
    {
        if (cols.find(a) == 0)
        {
            found_match = true;
            std::string substr{cols.substr(a.size(), cols.size())};
            bool substr_poss = is_possible(substr);
            poss[substr] = substr_poss;
            possible = possible || substr_poss;
        }
    }
    return possible && found_match;
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

    int sum{0};
    for (const std::string &col : colors)
    {
        bool possible = is_possible(col);
        if (possible)
        {
            sum++;
        }
        poss[col] = possible;
    }
    std::cout << sum << std::endl;

    return 0;
}
