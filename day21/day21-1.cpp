#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <cmath>
#include <climits>

struct pair_hash
{
    template <typename T1, typename T2>
    std::size_t operator()(const std::pair<T1, T2> &p) const
    {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 << 1);
    }
};

std::unordered_map<std::pair<char, char>, int, pair_hash> cost = {
    {{'A', 'A'}, 1},
    {{'A', '^'}, 2},
    {{'A', '>'}, 2},
    {{'A', 'v'}, 3},
    {{'A', '<'}, 4},
    {{'^', '^'}, 1},
    {{'^', 'A'}, 2},
    {{'^', 'v'}, 2},
    {{'^', '<'}, 3},
    {{'^', '>'}, 3},
    {{'<', '<'}, 1},
    {{'<', 'v'}, 2},
    {{'<', '>'}, 3},
    {{'<', '^'}, 3},
    {{'<', 'A'}, 4},
    {{'v', 'v'}, 1},
    {{'v', '<'}, 2},
    {{'v', '>'}, 2},
    {{'v', '^'}, 2},
    {{'v', 'A'}, 3},
    {{'>', '>'}, 1},
    {{'>', 'A'}, 2},
    {{'>', '^'}, 3},
    {{'>', 'v'}, 2},
    {{'>', '<'}, 3}};

std::vector<std::vector<char>> keypad = {
    {'7', '8', '9'},
    {'4', '5', '6'},
    {'1', '2', '3'},
    {' ', '0', 'A'}};

int k_start_x = 2;
int k_start_y = 3;

std::vector<std::vector<char>> dpad = {
    {' ', '^', 'A'},
    {'<', 'v', '>'}};
int d_start_x = 2;
int d_start_y = 0;

std::vector<std::string> search(std::vector<std::vector<char>> const &pad, int x, int y, char goal, std::string path, std::set<std::pair<int, int>> visited)
{
    std::vector<std::string> res;
    if (visited.count({x, y}))
    {
        return res;
    }
    int height = pad.size();
    int width = pad.back().size();
    visited.insert({x, y});
    if (x < 0 || x >= width || y < 0 || y >= height || pad[y][x] == ' ')
    {
        return res;
    }
    if (pad[y][x] == goal)
    {
        res.push_back(path + "A");
        return res;
    }
    std::vector<std::pair<std::pair<int, int>, char>> dirs = {{{0, 1}, 'v'}, {{1, 0}, '>'}, {{0, -1}, '^'}, {{-1, 0}, '<'}};
    for (auto const &dir : dirs)
    {
        int x_diff = dir.first.first;
        int y_diff = dir.first.second;
        char move = dir.second;
        if (x + x_diff < 0 || x + x_diff >= width || y + y_diff < 0 || y + y_diff >= height)
        {
            continue;
        }
        auto v = search(pad, x + x_diff, y + y_diff, goal, path + move, visited);
        res.insert(res.end(), v.begin(), v.end());
    }
    if (res.empty())
    {
        return res;
    }
    size_t min_length = std::min_element(res.begin(), res.end(), [](const std::string &a, const std::string &b)
                                         { return a.length() < b.length(); })
                            ->length();

    // Remove all elements that do not have the shortest length
    res.erase(std::remove_if(res.begin(), res.end(), [min_length](const std::string &str)
                             { return str.length() != min_length; }),
              res.end());
    return res;
}

std::pair<int, int> get_coords(std::vector<std::vector<char>> const &pad, char target)
{
    for (int y = 0; y < pad.size(); ++y)
    {
        for (int x = 0; x < pad[y].size(); ++x)
        {
            if (pad[y][x] == target)
            {
                return {x, y};
            }
        }
    }
    return {-1, -1};
}

std::vector<std::string> get_presses(std::vector<std::vector<char>> const &pad, std::string const &line, int start_x, int start_y)
{
    std::vector<std::string> presses = {""};
    std::pair<int, int> start = {start_x, start_y};
    for (char const &c : line)
    {
        std::vector<std::string> new_presses;
        for (auto const &press : presses)
        {
            auto res = search(pad, start.first, start.second, c, press, {});
            new_presses.insert(new_presses.begin(), res.begin(), res.end());
        }
        presses = new_presses;
        start = get_coords(pad, c);
    }
    return presses;
}

int main()
{
    std::ifstream file("input.txt");
    std::string line;
    int number;

    if (!file)
    {
        std::cerr << "Error opening the file!" << std::endl;
        return 1;
    }
    long long tot{0};
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        ss >> number;
        long long min_res = INT_MAX;
        auto first_presses = get_presses(keypad, line, k_start_x, k_start_y);
        for (auto const &p1 : first_presses)
        {
            auto second_presses = get_presses(dpad, p1, d_start_x, d_start_y);
            for (auto const &p2 : second_presses)
            {
                long long sum{0};
                char prev = dpad[d_start_y][d_start_x];
                for (char const &c : p2)
                {
                    sum += cost[{prev, c}];
                    prev = c;
                }
                min_res = std::min(sum, min_res);
            }
        }
        tot += min_res * number;
    }

    std::cout << tot << std::endl;

    file.close();

    return 0;
}
