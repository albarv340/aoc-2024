#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <utility>
#include <climits>
#include <algorithm>
#include <iomanip>

enum Direction
{
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 3
};

int height;
int width;
int start_x;
int start_y;
int end_x;
int end_y;
int min_dist;
std::vector<std::vector<char>> grid;
std::map<std::pair<int, int>, int> distance;
std::map<std::pair<int, int>, std::pair<int, int>> previous;
std::map<std::pair<int, int>, Direction> direction;
std::set<std::pair<int, int>> queue;
std::vector<std::pair<int, std::set<std::pair<int, int>>>> all;

int turns_needed(Direction start, Direction end)
{
    int clockwise_turns = (end - start + 4) % 4;
    int counter_clockwise_turns = (start - end + 4) % 4;

    return std::min(clockwise_turns, counter_clockwise_turns);
}

void update_if_cheaper(std::pair<int, int> min_dist_elem, int x, int y, int x_diff, int y_diff, Direction dir)
{
    if (queue.count({x + x_diff, y + y_diff}))
    {
        int turns = turns_needed(direction[min_dist_elem], dir);
        int new_dist = distance[{min_dist_elem}] + turns * 1000 + 1;
        if (new_dist < distance[{x + x_diff, y + y_diff}])
        {
            distance[{x + x_diff, y + y_diff}] = new_dist;
            previous[{x + x_diff, y + y_diff}] = min_dist_elem;
            direction[{x + x_diff, y + y_diff}] = dir;
        }
    }
}

void search(std::set<std::pair<int, int>> path, int x, int y, int dist, Direction direction)
{
    if (x < 0 || x >= width || y < 0 || y >= height || grid[y][x] == '#' || dist > min_dist || dist > distance[{x, y}] + 2000)
    {
        return;
    }
    Direction dir = EAST;
    int x_diff = 1;
    int y_diff = 0;
    int turns = turns_needed(direction, dir);
    int new_dist = dist + turns * 1000 + 1;
    if (!path.count({x + x_diff, y + y_diff}))
    {
        std::set<std::pair<int, int>> path_right(path);
        path_right.insert({x + x_diff, y + y_diff});
        if (x + x_diff == end_x && y + y_diff == end_y && new_dist == min_dist)
            all.push_back({new_dist, path_right});
        search(path_right, x + x_diff, y + y_diff, new_dist, dir);
    }
    dir = SOUTH;
    x_diff = 0;
    y_diff = 1;
    turns = turns_needed(direction, dir);
    new_dist = dist + turns * 1000 + 1;
    if (!path.count({x + x_diff, y + y_diff}))
    {
        std::set<std::pair<int, int>> path_down(path);
        path_down.insert({x + x_diff, y + y_diff});
        if (x + x_diff == end_x && y + y_diff == end_y && new_dist == min_dist)
            all.push_back({new_dist, path_down});
        search(path_down, x + x_diff, y + y_diff, new_dist, dir);
    }
    dir = WEST;
    x_diff = -1;
    y_diff = 0;
    turns = turns_needed(direction, dir);
    new_dist = dist + turns * 1000 + 1;
    if (!path.count({x + x_diff, y + y_diff}))
    {
        std::set<std::pair<int, int>> path_left(path);
        path_left.insert({x + x_diff, y + y_diff});
        if (x + x_diff == end_x && y + y_diff == end_y && new_dist == min_dist)
            all.push_back({new_dist, path_left});
        search(path_left, x + x_diff, y + y_diff, new_dist, dir);
    }
    dir = NORTH;
    x_diff = 0;
    y_diff = -1;
    turns = turns_needed(direction, dir);
    new_dist = dist + turns * 1000 + 1;
    if (!path.count({x + x_diff, y + y_diff}))
    {
        std::set<std::pair<int, int>> path_up(path);
        path_up.insert({x + x_diff, y + y_diff});
        if (x + x_diff == end_x && y + y_diff == end_y && new_dist == min_dist)
            all.push_back({new_dist, path_up});
        search(path_up, x + x_diff, y + y_diff, new_dist, dir);
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

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < height; x++)
        {
            distance[{x, y}] = INT_MAX;
            if (grid[y][x] != '#')
            {
                queue.insert({x, y});
            }
        }
    }

    distance[{start_x, start_y}] = 0;
    direction[{start_x, start_y}] = EAST;
    previous[{start_x, start_y}] = {-1, -1};

    while (!queue.empty())
    {
        int lowest_dist = INT_MAX;
        std::pair<int, int> min_dist_elem;
        for (const auto &loc : distance)
        {
            if (queue.count({loc.first}))
            {
                if (loc.second < lowest_dist)
                {
                    lowest_dist = loc.second;
                    min_dist_elem = loc.first;
                }
            }
        }
        queue.erase(min_dist_elem);

        int x = min_dist_elem.first;
        int y = min_dist_elem.second;
        update_if_cheaper(min_dist_elem, x, y, 1, 0, EAST);
        update_if_cheaper(min_dist_elem, x, y, 0, 1, SOUTH);
        update_if_cheaper(min_dist_elem, x, y, -1, 0, WEST);
        update_if_cheaper(min_dist_elem, x, y, 0, -1, NORTH);
    }

    long long sum{0};
    std::set<std::pair<int, int>> path;

    min_dist = distance[{end_x, end_y}];
    std::set<std::pair<int, int>> locs;

    search({}, start_x, start_y, 0, EAST);

    for (auto &f : all)
    {
        if (f.first == min_dist && f.second.count({end_x, end_y}))
        {
            for (auto &loc : f.second)
            {
                locs.insert(loc);
            }
        }
    }

    std::cout << locs.size() + 1 << std::endl; // +1 for start pos

    return 0;
}
