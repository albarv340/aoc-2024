#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <utility>
#include <climits>
#include <algorithm>

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
std::vector<std::vector<char>> grid;
std::map<std::pair<int, int>, int> distance;
std::map<std::pair<int, int>, std::pair<int, int>> previous;
std::map<std::pair<int, int>, Direction> direction;
std::set<std::pair<int, int>> queue;

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
        if (min_dist_elem.first == end_x && min_dist_elem.second == end_y)
        {
            break;
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
    std::pair<int, int> curr = previous[{end_x, end_y}];
    while (curr.first != start_x || curr.second != start_y)
    {
        path.insert(curr);
        curr = previous[curr];
    }

    
    std::cout << distance[{end_x, end_y}] << std::endl;

    return 0;
}
