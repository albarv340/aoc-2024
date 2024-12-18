#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <utility>
#include <climits>
#include <algorithm>
#include <sstream>

int height;
int width;
int start_x{0};
int start_y{0};
int end_x;
int end_y;
std::vector<std::vector<char>> grid;
std::map<std::pair<int, int>, int> distance;
std::map<std::pair<int, int>, std::pair<int, int>> previous;
std::set<std::pair<int, int>> queue;

void update_if_cheaper(std::pair<int, int> min_dist_elem, int x, int y, int x_diff, int y_diff)
{
    if (queue.count({x + x_diff, y + y_diff}))
    {
        int new_dist = distance[{min_dist_elem}] + 1;
        if (new_dist < distance[{x + x_diff, y + y_diff}])
        {
            distance[{x + x_diff, y + y_diff}] = new_dist;
            previous[{x + x_diff, y + y_diff}] = min_dist_elem;
        }
    }
}

int main()
{
    std::ifstream file("input.txt");
    height = 71;
    width = 71;
    end_x = width - 1;
    end_y = height - 1;
    if (!file)
    {
        std::cerr << "Could not open the file!" << std::endl;
        return 1;
    }

    std::string line;
    std::vector<std::pair<int, int>> bytes;
    int first, second;

    int insert_y = 0;
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        char delimiter;

        if (ss >> first >> delimiter >> second)
        {
            bytes.push_back({first, second});
        }
    }

    file.close();

    for (int y = 0; y < height; y++)
    {
        std::vector<char> row;
        for (int x = 0; x < width; x++)
        {
            row.push_back('.');
        }
        grid.push_back(row);
    }

    for (int i = 0; i < 2850; i++) // Implementation was so slow I did manual binary search to start at a place closer to the one that makes it impossible
    {
        int x = bytes[i].first;
        int y = bytes[i].second;
        grid[y][x] = '#';
    }
    for (int i = 2850; i < 5000; i++)
    {
        int first = bytes[i].first;
        int second = bytes[i].second;
        grid[second][first] = '#';
        previous.clear();

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                distance[{x, y}] = INT_MAX;
                if (grid[y][x] != '#')
                {
                    queue.insert({x, y});
                }
            }
        }
        distance[{start_x, start_y}] = 0;

        while (!queue.empty())
        {
            int lowest_dist = INT_MAX;
            std::pair<int, int> min_dist_elem = {-1, -1};
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
            if (min_dist_elem.first == end_x && min_dist_elem.second == end_y || min_dist_elem.first == -1)
            {
                break;
            }
            queue.erase(min_dist_elem);

            int x = min_dist_elem.first;
            int y = min_dist_elem.second;
            update_if_cheaper(min_dist_elem, x, y, 1, 0);
            update_if_cheaper(min_dist_elem, x, y, 0, 1);
            update_if_cheaper(min_dist_elem, x, y, -1, 0);
            update_if_cheaper(min_dist_elem, x, y, 0, -1);
        }

        if (!previous.count({end_x, end_y}))
        {
            std::cout << first << "," << second << std::endl;
            break;
        }
    }

    return 0;
}
