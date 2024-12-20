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
int no_cheat_dist{INT_MAX};
std::vector<std::vector<char>> grid;
std::map<std::pair<int, int>, int> distance;
std::set<std::pair<int, int>> visited;
std::set<std::pair<int, int>> end_path;

void search(std::set<std::pair<int, int>> path, int x, int y, int cheats)
{
    if (x < 0 || x >= width || y < 0 || y >= height)
    {
        return;
    }
    if (x == end_x && y == end_y)
    {
        end_path = path;
        distance[{x, y}] = path.size();
        return;
    }
    if (grid[y][x] == '#' && cheats >= 1)
    {
        return;
    }
    if (cheats == 2 && grid[y][x] != '#')
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
    if (grid[y][x] == '#' && cheats == 0)
    {
        cheats++;
        int x_diff = 1;
        int y_diff = 0;

        if (distance.count({x + x_diff, y + y_diff}) && (int)(distance[{x + x_diff, y + y_diff}] - path.size()) >= 100)
        {
            visited.insert({x, y});
        }
        x_diff = 0;
        y_diff = 1;
        if (distance.count({x + x_diff, y + y_diff}) && (int)(distance[{x + x_diff, y + y_diff}] - path.size()) >= 100)
        {
            visited.insert({x, y});
        }
        x_diff = -1;
        y_diff = 0;
        if (distance.count({x + x_diff, y + y_diff}) && (int)(distance[{x + x_diff, y + y_diff}] - path.size()) >= 100)
        {
            visited.insert({x, y});
        }
        x_diff = 0;
        y_diff = -1;
        if (distance.count({x + x_diff, y + y_diff}) && (int)(distance[{x + x_diff, y + y_diff}] - path.size()) >= 100)
        {
            visited.insert({x, y});
        }
        return;
    }
    int x_diff = 1;
    int y_diff = 0;
    if (!path.count({x + x_diff, y + y_diff}))
    {
        std::set<std::pair<int, int>> new_path(path);
        new_path.insert({x + x_diff, y + y_diff});
        search(new_path, x + x_diff, y + y_diff, cheats);
    }
    x_diff = 0;
    y_diff = 1;
    if (!path.count({x + x_diff, y + y_diff}))
    {
        std::set<std::pair<int, int>> new_path(path);
        new_path.insert({x + x_diff, y + y_diff});
        search(new_path, x + x_diff, y + y_diff, cheats);
    }
    x_diff = -1;
    y_diff = 0;
    if (!path.count({x + x_diff, y + y_diff}))
    {
        std::set<std::pair<int, int>> new_path(path);
        new_path.insert({x + x_diff, y + y_diff});
        search(new_path, x + x_diff, y + y_diff, cheats);
    }
    x_diff = 0;
    y_diff = -1;
    if (!path.count({x + x_diff, y + y_diff}))
    {
        std::set<std::pair<int, int>> new_path(path);
        new_path.insert({x + x_diff, y + y_diff});
        search(new_path, x + x_diff, y + y_diff, cheats);
    }
}

int main()
{
    // std::ifstream file("sample.txt");
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

    search({}, start_x, start_y, 2);

    no_cheat_dist = end_path.size();

    search({}, start_x, start_y, 0);

    std::cout << visited.size() << std::endl;

    return 0;
}
