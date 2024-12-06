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

    std::set<std::pair<int, int>> visited_coordinates;

    bool has_guard_left = false;
    while (!has_guard_left)
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
                grid[guard_y][guard_x] = 'X';
                visited_coordinates.insert({guard_x, guard_y});
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
                grid[guard_y][guard_x] = 'X';
                visited_coordinates.insert({guard_x, guard_y});
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
                grid[guard_y][guard_x] = 'X';
                visited_coordinates.insert({guard_x, guard_y});
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
                grid[guard_y][guard_x] = 'X';
                visited_coordinates.insert({guard_x, guard_y});
            }
            break;
        default:
            break;
        }
    }

    std::cout << visited_coordinates.size() + 1 << std::endl; // +1 for the start position

    return 0;
}
