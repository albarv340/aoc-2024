#include <iostream>
#include <fstream>
#include <regex>
#include <string>

int main()
{
    std::ifstream file("input.txt");

    if (!file.is_open())
    {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    std::string input((std::istreambuf_iterator<char>(file)),
                      std::istreambuf_iterator<char>());

    file.close();
    std::regex replace_pattern("don't\\(\\)[^]*?(?:do\\(\\)|$)");
    std::string result = std::regex_replace(input, replace_pattern, "");

    std::regex pattern("mul\\((\\d+),(\\d+)\\)");

    auto matches_begin = std::sregex_iterator(result.begin(), result.end(), pattern);
    auto matches_end = std::sregex_iterator();

    int sum{0};

    for (std::sregex_iterator i = matches_begin; i != matches_end; ++i)
    {
        std::smatch match = *i;
        if (match.size() > 1)
        {
            sum += std::stoi(match.str(1)) * std::stoi(match.str(2));
        }
    }

    std::cout << sum << std::endl;

    return 0;
}
