#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <map>
#include <set>
#include <climits>

std::map<std::pair<int, int>, long long> price_map;

std::set<std::pair<int, int>> visited;

void search(int x, int y, int button_a_x, int button_a_y, int button_b_x, int button_b_y, int prize_x, int prize_y, int a_clicks, int b_clicks)
{
    if (visited.count({x, y}))
    {
        return;
    }
    visited.insert({x, y});
    if (x > prize_x || y > prize_y)
    {
        return;
    }
    if (x == prize_x && y == prize_y)
    {
        price_map[{a_clicks, b_clicks}] = 3 * a_clicks + b_clicks;
        return;
    }
    search(x + button_a_x, y + button_a_y, button_a_x, button_a_y, button_b_x, button_b_y, prize_x, prize_y, a_clicks + 1, b_clicks);
    search(x + button_b_x, y + button_b_y, button_a_x, button_a_y, button_b_x, button_b_y, prize_x, prize_y, a_clicks, b_clicks + 1);
}

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

    int button_a_x;
    int button_a_y;
    int button_b_x;
    int button_b_y;
    int prize_x;
    int prize_y;

    long long sum{0};
    std::smatch match;
    while (std::getline(file, line))
    {
        if (std::regex_search(line, match, button_a_regex))
        {
            button_a_x = std::stoi(match[1].str());
            button_a_y = std::stoi(match[2].str());
        }
        else if (std::regex_search(line, match, button_b_regex))
        {
            button_b_x = std::stoi(match[1].str());
            button_b_y = std::stoi(match[2].str());
        }
        else if (std::regex_search(line, match, prize_regex))
        {
            prize_x = std::stoi(match[1].str());
            prize_y = std::stoi(match[2].str());
        }
        if (line.empty())
        {
            price_map.clear();
            visited.clear();
            search(0, 0, button_a_x, button_a_y, button_b_x, button_b_y, prize_x, prize_y, 0, 0);
            int min = INT_MAX;
            for (auto &elem : price_map)
            {
                if (elem.second < min)
                {
                    min = elem.second;
                }
            }
            if (min != INT_MAX)
            {
                sum += min;
            }
        }
    }

    std::cout << sum << std::endl;

    return 0;
}
