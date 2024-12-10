#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <cmath>
#include <set>

int width;
int height;

int get_score(std::vector<std::vector<int>> const &grid, int x, int y)
{
    int sum = 0;
    if (grid[y][x] == 9)
    {
        return 1;
    }
    if (x > 0 && grid[y][x - 1] == grid[y][x] + 1)
    {
        sum += get_score(grid, x - 1, y);
    }
    if (y > 0 && grid[y - 1][x] == grid[y][x] + 1)
    {
        sum += get_score(grid, x, y - 1);
    }
    if (x + 1 < width && grid[y][x + 1] == grid[y][x] + 1)
    {
        sum += get_score(grid, x + 1, y);
    }
    if (y + 1 < height && grid[y + 1][x] == grid[y][x] + 1)
    {
        sum += get_score(grid, x, y + 1);
    }

    return sum;
}

int main()
{
    std::ifstream file("input.txt");

    if (!file.is_open())
    {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    std::vector<std::vector<int>> grid;
    std::string line;
    int row = 0;

    while (getline(file, line))
    {
        std::vector<int> grid_row;
        for (int col = 0; col < line.length(); col++)
        {
            int current_char = line[col] - '0';
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
            if (grid[y][x] != 0)
                continue;
            int score = get_score(grid, x, y);
            sum += score;
        }
    }
    std::cout << sum << std::endl;
}
