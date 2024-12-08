#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <cmath>
#include <set>

using namespace std;

bool are_collinear(pair<int, int> const &p1, pair<int, int> const &p2, pair<int, int> const &p3)
{
    return (p1.first - p2.first) * (p2.second - p3.second) == (p1.second - p2.second) * (p2.first - p3.first);
}

int main()
{
    ifstream file("input.txt");

    if (!file.is_open())
    {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    vector<vector<char>> grid;
    unordered_map<char, vector<pair<int, int>>> antenna_locations;
    string line;
    int row = 0;

    while (getline(file, line))
    {
        vector<char> grid_row;
        for (int col = 0; col < line.length(); col++)
        {
            char current_char = line[col];
            grid_row.push_back(current_char);

            if (current_char != '.')
            {
                antenna_locations[current_char].push_back({row, col});
            }
        }
        grid.push_back(grid_row);
        row++;
    }

    file.close();

    int width = grid[0].size();
    int height = row;

    std::set<std::pair<int, int>> antinodes;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            for (const auto &pair : antenna_locations)
            {
                char frequency = pair.first;
                auto const &values = pair.second;
                for (const auto &location1 : values)
                {
                    for (const auto &location2 : values)
                    {
                        if (location1.first == location2.first && location1.second == location2.second)
                            continue;
                        if (!are_collinear({x, y}, location1, location2))
                            continue;
                        antinodes.insert({x, y});
                    }
                }
            }
        }
    }

    std::cout << antinodes.size() << std::endl;

    return 0;
}
