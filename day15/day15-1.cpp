#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <utility>

int main()
{
    std::ifstream file("input.txt");
    if (!file)
    {
        std::cerr << "Could not open the file!" << std::endl;
        return 1;
    }

    std::vector<std::vector<char>> grid;
    std::vector<char> moves;
    std::string line;
    int robot_x = -1;
    int robot_y = -1;
    int height;
    int width;

    int insert_y = 0;
    bool has_reached_moves = false;
    while (std::getline(file, line))
    {
        if (line.empty())
        {
            has_reached_moves = true;
            continue;
        }
        if (!has_reached_moves)
        {
            std::vector<char> row;

            for (int insert_x = 0; insert_x < line.size(); ++insert_x)
            {
                char c = line[insert_x];
                row.push_back(c);

                if (c == '@')
                {
                    robot_x = insert_x;
                    robot_y = insert_y;
                }
            }

            grid.push_back(row);
            ++insert_y;
        }
        else
        {
            for (char move : line)
            {
                moves.push_back(move);
            }
        }
    }

    width = grid[0].size();
    height = insert_y;
    file.close();

    for (auto &move : moves)
    {
        switch (move)
        {
        case '^':
            if (grid[robot_y - 1][robot_x] != '.')
            {
                bool found_space = false;
                int i;
                for (i = 1; i < robot_y; i++)
                {
                    if (grid[robot_y - i][robot_x] == '#')
                    {
                        break;
                    }
                    if (grid[robot_y - i][robot_x] == '.')
                    {
                        found_space = true;
                        break;
                    }
                }
                if (found_space)
                {
                    for (int j = i; j > 0; j--)
                    {
                        grid[robot_y - j][robot_x] = grid[robot_y - j + 1][robot_x];
                    }
                    grid[robot_y][robot_x] = '.';
                    robot_y = robot_y - 1;
                }
            }
            else
            {
                grid[robot_y - 1][robot_x] = '@';
                grid[robot_y][robot_x] = '.';
                robot_y = robot_y - 1;
            }
            break;
        case '>':
            if (grid[robot_y][robot_x + 1] != '.')
            {
                bool found_space = false;
                int i;
                for (i = 1; i < width - robot_x; i++)
                {
                    if (grid[robot_y][robot_x + i] == '#')
                    {
                        break;
                    }
                    if (grid[robot_y][robot_x + i] == '.')
                    {
                        found_space = true;
                        break;
                    }
                }
                if (found_space)
                {
                    for (int j = i; j > 0; j--)
                    {
                        grid[robot_y][robot_x + j] = grid[robot_y][robot_x + j - 1];
                    }
                    grid[robot_y][robot_x] = '.';
                    robot_x = robot_x + 1;
                }
            }
            else
            {
                grid[robot_y][robot_x + 1] = '@';
                grid[robot_y][robot_x] = '.';
                robot_x = robot_x + 1;
            }
            break;
        case 'v':
            if (grid[robot_y + 1][robot_x] != '.')
            {
                bool found_space = false;
                int i;
                for (i = 1; i < height - robot_y; i++)
                {
                    if (grid[robot_y + i][robot_x] == '#')
                    {
                        break;
                    }
                    if (grid[robot_y + i][robot_x] == '.')
                    {
                        found_space = true;
                        break;
                    }
                }
                if (found_space)
                {
                    for (int j = i; j > 0; j--)
                    {
                        grid[robot_y + j][robot_x] = grid[robot_y + j - 1][robot_x];
                    }
                    grid[robot_y][robot_x] = '.';
                    robot_y = robot_y + 1;
                }
            }
            else
            {
                grid[robot_y + 1][robot_x] = '@';
                grid[robot_y][robot_x] = '.';
                robot_y = robot_y + 1;
            }
            break;
        case '<':
            if (grid[robot_y][robot_x - 1] != '.')
            {
                bool found_space = false;
                int i;
                for (i = 1; i < robot_x; i++)
                {
                    if (grid[robot_y][robot_x - i] == '#')
                    {
                        break;
                    }
                    if (grid[robot_y][robot_x - i] == '.')
                    {
                        found_space = true;
                        break;
                    }
                }
                if (found_space)
                {
                    for (int j = i; j > 0; j--)
                    {
                        grid[robot_y][robot_x - j] = grid[robot_y][robot_x - j + 1];
                    }
                    grid[robot_y][robot_x] = '.';
                    robot_x = robot_x - 1;
                }
            }
            else
            {
                grid[robot_y][robot_x - 1] = '@';
                grid[robot_y][robot_x] = '.';
                robot_x = robot_x - 1;
            }
            break;
        default:
            break;
        }
    }
    long long sum{0};
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < height; x++)
        {
            if (grid[y][x] == 'O')
            {
                sum += 100 * y + x;
            }
        }
    }
    std::cout << sum << std::endl;

    return 0;
}
