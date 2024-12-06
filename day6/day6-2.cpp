#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <utility>

enum Direction
{
    UP,
    RIGHT,
    LEFT,
    DOWN
};

int main()
{
    std::ifstream file("input.txt");
    if (!file)
    {
        std::cerr << "Could not open the file!" << std::endl;
        return 1;
    }

    std::vector<std::vector<char>> grid;
    std::string line;
    int guard_x = -1;
    int guard_y = -1;
    int height;
    int width;
    Direction direction = UP;

    int insert_y = 0;
    while (std::getline(file, line))
    {
        std::vector<char> row;

        for (int insert_x = 0; insert_x < line.size(); ++insert_x)
        {
            char c = line[insert_x];
            row.push_back(c);

            if (c == '^')
            {
                guard_x = insert_x;
                guard_y = insert_y;
            }
        }

        grid.push_back(row);
        ++insert_y;
    }

    width = grid[0].size();
    height = insert_y;
    file.close();

    int sum{0};
    int original_guard_x = guard_x;
    int original_guard_y = guard_y;
    Direction original_direction = direction;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            bool has_guard_left = false;
            int total_moves{0};
            char original = grid[y][x];
            if (original != '.') continue;
            grid[y][x] = '#';
            while (!has_guard_left && total_moves < height * width)
            {
                switch (direction)
                {
                case UP:
                    if (guard_y - 1 < 0)
                    {
                        has_guard_left = true;
                    }
                    else if (grid[guard_y - 1][guard_x] == '#')
                    {
                        direction = RIGHT;
                    }
                    else
                    {
                        guard_y = guard_y - 1;
                    }
                    break;
                case RIGHT:
                    if (guard_x + 1 >= width)
                    {
                        has_guard_left = true;
                    }
                    else if (grid[guard_y][guard_x + 1] == '#')
                    {
                        direction = DOWN;
                    }
                    else
                    {
                        guard_x = guard_x + 1;
                    }
                    break;
                case DOWN:
                    if (guard_y + 1 >= height)
                    {
                        has_guard_left = true;
                    }
                    else if (grid[guard_y + 1][guard_x] == '#')
                    {
                        direction = LEFT;
                    }
                    else
                    {
                        guard_y = guard_y + 1;
                    }
                    break;
                case LEFT:
                    if (guard_x - 1 < 0)
                    {
                        has_guard_left = true;
                    }
                    else if (grid[guard_y][guard_x - 1] == '#')
                    {
                        direction = UP;
                    }
                    else
                    {
                        guard_x = guard_x - 1;
                    }
                    break;
                default:
                    break;
                }
                total_moves++;
            }
            if (!has_guard_left) {
                sum++;
            }
            grid[y][x] = original;
            guard_x = original_guard_x;
            guard_y = original_guard_y;
            direction = original_direction;
        }
    }

    std::cout << sum << std::endl;

    return 0;
}
