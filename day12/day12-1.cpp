#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <cmath>
#include <set>

int width;
int height;

std::set<std::pair<int, int>> checked_chars;

std::pair<int, int> get_area_and_per(std::vector<std::vector<char>> const &grid, int x, int y)
{
    if (checked_chars.count({x, y}))
    {
        return {0, 0};
    }
    checked_chars.insert({x, y});
    std::pair<int, int> area_and_per{0, 0};
    area_and_per.first++;
    if (x > 0 && grid[y][x - 1] == grid[y][x])
    {
        auto other = get_area_and_per(grid, x - 1, y);
        area_and_per.first += other.first;
        area_and_per.second += other.second;
    }
    else
    {
        area_and_per.second++;
    }
    if (y > 0 && grid[y - 1][x] == grid[y][x])
    {
        auto other = get_area_and_per(grid, x, y - 1);
        area_and_per.first += other.first;
        area_and_per.second += other.second;
    }
    else
    {
        area_and_per.second++;
    }
    if (x + 1 < width && grid[y][x + 1] == grid[y][x])
    {
        auto other = get_area_and_per(grid, x + 1, y);
        area_and_per.first += other.first;
        area_and_per.second += other.second;
    }
    else
    {
        area_and_per.second++;
    }
    if (y + 1 < height && grid[y + 1][x] == grid[y][x])
    {
        auto other = get_area_and_per(grid, x, y + 1);
        area_and_per.first += other.first;
        area_and_per.second += other.second;
    }
    else
    {
        area_and_per.second++;
    }

    return area_and_per;
}

int main()
{
    std::ifstream file("input.txt");

    if (!file.is_open())
    {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    std::vector<std::vector<char>> grid;
    std::string line;
    int row = 0;

    while (getline(file, line))
    {
        std::vector<char> grid_row;
        for (int col = 0; col < line.length(); col++)
        {
            char current_char = line[col];
            grid_row.push_back(current_char);
        }
        grid.push_back(grid_row);
        row++;
    }

    file.close();

    width = grid[0].size();
    height = row;
    long long sum{0};
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (checked_chars.count({x, y}))
            {
                continue;
            }
            auto res = get_area_and_per(grid, x, y);
            int score = res.first * res.second;
            sum += score;
        }
    }
    std::cout << sum << std::endl;
}
