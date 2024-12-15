#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <utility>

int height;
int width;

std::set<std::pair<int, int>> visited;

bool traverse_vert(std::vector<std::vector<char>> const &grid, std::vector<std::vector<bool>> &move_mask, int x, int y, int y_diff)
{
    bool res = true;
    if (visited.count({x, y}))
    {
        return true;
    }
    visited.insert({x, y});
    if (y < 0 || y >= height)
    {
        return true;
    }
    if (grid[y][x] == '.')
    {
        return true;
    }
    if (grid[y][x] == '[')
    {
        move_mask[y][x] = 1;
        res = res && traverse_vert(grid, move_mask, x + 1, y, y_diff);
        res = res && traverse_vert(grid, move_mask, x, y + y_diff, y_diff);
    }
    if (grid[y][x] == ']')
    {
        move_mask[y][x] = 1;
        res = res && traverse_vert(grid, move_mask, x - 1, y, y_diff);
        res = res && traverse_vert(grid, move_mask, x, y + y_diff, y_diff);
    }
    if (grid[y][x] == '#')
    {
        return false;
    }
    return res;
}

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

                if (c == '@')
                {
                    robot_x = insert_x * 2;
                    robot_y = insert_y;
                    row.push_back(c);
                    row.push_back('.');
                }
                else if (c == 'O')
                {
                    row.push_back('[');
                    row.push_back(']');
                }
                else
                {
                    row.push_back(c);
                    row.push_back(c);
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
                visited.clear();
                std::vector<std::vector<bool>> move_mask{0};
                for (int y = 0; y < height; y++)
                {
                    std::vector<bool> row;
                    for (int x = 0; x < width; x++)
                    {
                        row.push_back(false);
                    }
                    move_mask.push_back(row);
                }
                bool should_move = traverse_vert(grid, move_mask, robot_x, robot_y - 1, -1);
                if (should_move)
                {
                    for (int y = 0; y < height; y++)
                    {
                        for (int x = 0; x < width; x++)
                        {
                            if (move_mask[y][x])
                            {
                                grid[y - 1][x] = grid[y][x];
                                grid[y][x] = '.';
                            }
                        }
                    }
                    grid[robot_y - 1][robot_x] = '@';
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
                visited.clear();
                std::vector<std::vector<bool>> move_mask{0};
                for (int y = 0; y < height; y++)
                {
                    std::vector<bool> row;
                    for (int x = 0; x < width; x++)
                    {
                        row.push_back(false);
                    }
                    move_mask.push_back(row);
                }
                bool should_move = traverse_vert(grid, move_mask, robot_x, robot_y + 1, 1);
                if (should_move)
                {
                    for (int y = height - 1; y >= 0; y--)
                    {
                        for (int x = 0; x < width; x++)
                        {
                            if (move_mask[y][x])
                            {
                                grid[y + 1][x] = grid[y][x];
                                grid[y][x] = '.';
                            }
                        }
                    }
                    grid[robot_y + 1][robot_x] = '@';
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
        for (int x = 0; x < width; x++)
        {
            if (grid[y][x] == '[')
            {
                sum += 100 * y + x;
            }
        }
    }
    std::cout << sum << std::endl;

    return 0;
}
