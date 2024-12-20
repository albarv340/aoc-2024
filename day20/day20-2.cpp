#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <utility>
#include <climits>
#include <algorithm>

int height;
int width;
int start_x;
int start_y;
int end_x;
int end_y;
std::vector<std::vector<char>> grid;
std::map<std::pair<int, int>, int> distance;
std::set<std::pair<std::pair<int, int>, std::pair<int, int>>> visited;

void search(std::set<std::pair<int, int>> path, int x, int y)
{
    if (x < 0 || x >= width || y < 0 || y >= height)
    {
        return;
    }
    if (grid[y][x] == '#')
    {
        return;
    }

    if (x == end_x && y == end_y)
    {
        distance[{x, y}] = path.size();
        return;
    }

    if (grid[y][x] != '#')
    {
        if (distance.count({x, y}))
        {
            if (path.size() < distance[{x, y}])
            {
                distance[{x, y}] = path.size();
            }
        }
        else
        {
            distance[{x, y}] = path.size();
        }
    }

    int x_diff = 1;
    int y_diff = 0;
    if (!path.count({x + x_diff, y + y_diff}))
    {
        std::set<std::pair<int, int>> new_path(path);
        new_path.insert({x + x_diff, y + y_diff});
        search(new_path, x + x_diff, y + y_diff);
    }
    x_diff = 0;
    y_diff = 1;
    if (!path.count({x + x_diff, y + y_diff}))
    {
        std::set<std::pair<int, int>> new_path(path);
        new_path.insert({x + x_diff, y + y_diff});
        search(new_path, x + x_diff, y + y_diff);
    }
    x_diff = -1;
    y_diff = 0;
    if (!path.count({x + x_diff, y + y_diff}))
    {
        std::set<std::pair<int, int>> new_path(path);
        new_path.insert({x + x_diff, y + y_diff});
        search(new_path, x + x_diff, y + y_diff);
    }
    x_diff = 0;
    y_diff = -1;
    if (!path.count({x + x_diff, y + y_diff}))
    {
        std::set<std::pair<int, int>> new_path(path);
        new_path.insert({x + x_diff, y + y_diff});
        search(new_path, x + x_diff, y + y_diff);
    }
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

    int insert_y = 0;
    while (std::getline(file, line))
    {
        std::vector<char> row;

        for (int insert_x = 0; insert_x < line.size(); ++insert_x)
        {
            char c = line[insert_x];
            row.push_back(c);

            if (c == 'S')
            {
                start_x = insert_x;
                start_y = insert_y;
            }

            if (c == 'E')
            {
                end_x = insert_x;
                end_y = insert_y;
            }
        }

        grid.push_back(row);
        ++insert_y;
    }

    width = grid[0].size();
    height = insert_y;
    file.close();

    search({}, start_x, start_y);

    std::map<int, int> pairs;
    long long sum{0};

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            for (int x_diff = -20; x_diff < 21; x_diff++)
            {
                for (int y_diff = -20; y_diff < 21; y_diff++)
                {
                    int cheat_steps = abs(y_diff) + abs(x_diff);
                    if (cheat_steps > 20 || x + x_diff < 0 || x + x_diff >= width || y + y_diff < 0 || y + y_diff >= height || grid[y][x] == '#' || grid[y + y_diff][x + x_diff] == '#')
                    {
                        continue;
                    }
                    if ((distance[{x + x_diff, y + y_diff}] - (distance[{x, y}] + cheat_steps)) >= 100)
                    {
                        sum++;
                    }
                }
            }
        }
    }

    std::cout << sum << std::endl;

    return 0;
}
