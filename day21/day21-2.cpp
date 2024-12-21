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

std::unordered_map<std::pair<char, char>, long long, pair_hash> cost = {
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

long long k_start_x = 2;
long long k_start_y = 3;

std::vector<std::vector<char>> dpad = {
    {' ', '^', 'A'},
    {'<', 'v', '>'}};
long long d_start_x = 2;
long long d_start_y = 0;

std::vector<std::string> search(std::vector<std::vector<char>> const &pad, long long x, long long y, char goal, std::string path, std::set<std::pair<long long, long long>> visited)
{
    std::vector<std::string> res;
    if (visited.count({x, y}))
    {
        return res;
    }
    long long height = pad.size();
    long long width = pad.back().size();
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
    std::vector<std::pair<std::pair<long long, long long>, char>> dirs = {{{0, 1}, 'v'}, {{1, 0}, '>'}, {{0, -1}, '^'}, {{-1, 0}, '<'}};
    for (auto const &dir : dirs)
    {
        long long x_diff = dir.first.first;
        long long y_diff = dir.first.second;
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

std::pair<long long, long long> get_coords(std::vector<std::vector<char>> const &pad, char target)
{
    for (long long y = 0; y < pad.size(); ++y)
    {
        for (long long x = 0; x < pad[y].size(); ++x)
        {
            if (pad[y][x] == target)
            {
                return {x, y};
            }
        }
    }
    return {-1, -1};
}

std::vector<std::string> get_presses(std::vector<std::vector<char>> const &pad, std::string const &line, long long start_x, long long start_y)
{
    std::vector<std::string> presses = {""};
    std::pair<long long, long long> start = {start_x, start_y};
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

std::unordered_map<std::pair<char, char>, std::string, pair_hash> paths;
std::string get_path(std::vector<std::vector<char>> const &pad, long long x, long long y, char e)
{
    auto res = search(pad, x, y, e, "", {});
    long long min_dist{INT_LEAST64_MAX};
    std::string res_s;
    for (auto const &r : res)
    {
        long long sum{0};
        char p = 'A';
        for (auto const c : r)
        {
            sum += cost[{p, c}];
            p = c;
        }
        if (sum < min_dist)
        {
            res_s = r;
            min_dist = sum;
        }
    }
    paths[{pad[y][x], e}] = res_s;
    return res_s;
}

int main()
{
    std::ifstream file("input.txt");
    std::string line;
    long long number;

    if (!file)
    {
        std::cerr << "Error opening the file!" << std::endl;
        return 1;
    }
    long long tot{0};
    for (long long i = 0; i < 24; i++) // Cost is initialized to first robot's price
    {
        std::unordered_map<std::pair<char, char>, long long, pair_hash> new_cost;
        for (auto const &co : cost)
        {
            auto coord{get_coords(dpad, co.first.first)};
            auto string{get_path(dpad, coord.first, coord.second, co.first.second)};
            char prev = 'A';
            for (auto const ch : string)
            {
                new_cost[{co.first}] += cost[{prev, ch}];
                prev = ch;
            }
        }
        cost = new_cost;
    }
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        ss >> number;
        long long min_res = INT_MAX;
        auto presses = get_presses(keypad, line, k_start_x, k_start_y);
        long long min_dist{INT_LEAST64_MAX};
        for (auto const &r : presses)
        {
            long long sum{0};
            char p = 'A';
            for (auto const c : r)
            {
                sum += cost[{p, c}];
                p = c;
            }
            if (sum < min_dist)
            {
                min_dist = sum;
            }
        }
        tot += min_dist * number;
    }

    std::cout << tot << std::endl;

    file.close();

    return 0;
}
