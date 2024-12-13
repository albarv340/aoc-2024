#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

int main()
{
    std::ifstream file("input.txt");
    if (!file)
    {
        std::cerr << "Could not open the file!" << std::endl;
        return 1;
    }

    std::string line;

    std::regex button_a_regex("Button A: X\\+(\\d+), Y\\+(\\d+)");
    std::regex button_b_regex("Button B: X\\+(\\d+), Y\\+(\\d+)");
    std::regex prize_regex("Prize: X=(\\d+), Y=(\\d+)");

    long long button_a_x;
    long long button_a_y;
    long long button_b_x;
    long long button_b_y;
    long long prize_x;
    long long prize_y;

    long long sum{0};
    std::smatch match;
    while (std::getline(file, line))
    {
        if (std::regex_search(line, match, button_a_regex))
        {
            button_a_x = std::stoll(match[1].str());
            button_a_y = std::stoll(match[2].str());
        }
        else if (std::regex_search(line, match, button_b_regex))
        {
            button_b_x = std::stoll(match[1].str());
            button_b_y = std::stoll(match[2].str());
        }
        else if (std::regex_search(line, match, prize_regex))
        {
            prize_x = std::stoll(match[1].str()) + 10000000000000;
            prize_y = std::stoll(match[2].str()) + 10000000000000;
        }
        if (line.empty())
        {
            long long determinant = button_a_x * button_b_y - button_a_y * button_b_x;
            if (determinant != 0)
            {
                if ((prize_x * button_b_y - button_b_x * prize_y) % determinant == 0 && (button_a_x * prize_y - prize_x * button_a_y) % determinant == 0)
                {
                    long long a = (prize_x * button_b_y - button_b_x * prize_y) / determinant;
                    long long b = (button_a_x * prize_y - prize_x * button_a_y) / determinant;
                    sum += 3 * a + b;
                }
            }
        }
    }
    std::cout << sum << std::endl;

    return 0;
}
