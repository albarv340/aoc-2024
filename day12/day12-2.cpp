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

std::pair<int, double> get_area_and_per(std::vector<std::vector<char>> const &grid, int x, int y)
{
    if (checked_chars.count({x, y}))
    {
        return {0, 0};
    }
    checked_chars.insert({x, y});
    std::pair<int, double> area_and_per{0, 0};
    area_and_per.first++;
    if (x > 0 && grid[y][x - 1] == grid[y][x])
    {
        auto other = get_area_and_per(grid, x - 1, y);
        area_and_per.first += other.first;
        area_and_per.second += other.second;
    }
    else
    {
        int side_length = 1;
        int pos_y_offset = 1;
        while (y + pos_y_offset < height && grid[y + pos_y_offset][x] == grid[y][x] && (x == 0 || grid[y + pos_y_offset][x - 1] != grid[y][x]))
        {
            side_length++;
            pos_y_offset++;
        }
        int neg_y_offset = -1;
        while (y + neg_y_offset >= 0 && grid[y + neg_y_offset][x] == grid[y][x] && (x == 0 || grid[y + neg_y_offset][x - 1] != grid[y][x]))
        {
            side_length++;
            neg_y_offset--;
        }

        area_and_per.second += 1.0 / side_length;
    }
    if (y > 0 && grid[y - 1][x] == grid[y][x])
    {
        auto other = get_area_and_per(grid, x, y - 1);
        area_and_per.first += other.first;
        area_and_per.second += other.second;
    }
    else
    {
        int side_length = 1;
        int pos_x_offset = 1;
        while (x + pos_x_offset < width && grid[y][x + pos_x_offset] == grid[y][x] && (y == 0 || grid[y - 1][x + pos_x_offset] != grid[y][x]))
        {
            side_length++;
            pos_x_offset++;
        }
        int neg_x_offset = -1;
        while (x + neg_x_offset >= 0 && grid[y][x + neg_x_offset] == grid[y][x] && (y == 0 || grid[y - 1][x + neg_x_offset] != grid[y][x]))
        {
            side_length++;
            neg_x_offset--;
        }
        area_and_per.second += 1.0 / side_length;
    }
    if (x + 1 < width && grid[y][x + 1] == grid[y][x])
    {
        auto other = get_area_and_per(grid, x + 1, y);
        area_and_per.first += other.first;
        area_and_per.second += other.second;
    }
    else
    {
        int side_length = 1;
        int pos_y_offset = 1;
        while (y + pos_y_offset < height && grid[y + pos_y_offset][x] == grid[y][x] && (x == width - 1 || grid[y + pos_y_offset][x + 1] != grid[y][x]))
        {
            side_length++;
            pos_y_offset++;
        }
        int neg_y_offset = -1;
        while (y + neg_y_offset >= 0 && grid[y + neg_y_offset][x] == grid[y][x] && (x == width - 1 || grid[y + neg_y_offset][x + 1] != grid[y][x]))
        {
            side_length++;
            neg_y_offset--;
        }
        area_and_per.second += 1.0 / side_length;
    }
    if (y + 1 < height && grid[y + 1][x] == grid[y][x])
    {
        auto other = get_area_and_per(grid, x, y + 1);
        area_and_per.first += other.first;
        area_and_per.second += other.second;
    }
    else
    {
        int side_length = 1;
        int pos_x_offset = 1;
        while (x + pos_x_offset < width && grid[y][x + pos_x_offset] == grid[y][x] && (y == height - 1 || grid[y + 1][x + pos_x_offset] != grid[y][x]))
        {
            side_length++;
            pos_x_offset++;
        }
        int neg_x_offset = -1;
        while (x + neg_x_offset >= 0 && grid[y][x + neg_x_offset] == grid[y][x] && (y == height - 1 || grid[y + 1][x + neg_x_offset] != grid[y][x]))
        {
            side_length++;
            neg_x_offset--;
        }
        area_and_per.second += 1.0 / side_length;
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
            int score = res.first * round(res.second);
            sum += score;
        }
    }
    std::cout << sum << std::endl;
}
