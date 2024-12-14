#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <map>
#include <set>

int main()
{
    std::ifstream file("input.txt");
    int width = 101;
    int height = 103;
    if (!file)
    {
        std::cerr << "Could not open the file!" << std::endl;
        return 1;
    }

    std::map<std::pair<int, int>, int> locations;
    std::regex regex("p=(\\d+),(\\d+) v=(-?\\d+),(-?\\d+)");
    std::string line;

    long long sum{0};
    std::smatch match;
    while (std::getline(file, line))
    {
        if (std::regex_search(line, match, regex))
        {
            int x = std::stoi(match[1].str());
            int y = std::stoi(match[2].str());
            int vx = std::stoi(match[3].str());
            int vy = std::stoi(match[4].str());
            std::pair<int, int> loc{(((x + 100 * vx) % width) + width) % width, (((y + 100 * vy) % height) + height) % height};
            locations[loc] += 1;
        }
    }

    int q1{0};
    int q2{0};
    int q3{0};
    int q4{0};

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            if (y == height / 2 || x == width / 2)
                continue;
            if (x < width / 2 && y > height / 2)
            {
                q1 += locations[{x, y}];
            }
            if (x < width / 2 && y < height / 2)
            {
                q3 += locations[{x, y}];
            }
            if (x > width / 2 && y > height / 2)
            {
                q2 += locations[{x, y}];
            }
            if (x > width / 2 && y < height / 2)
            {
                q4 += locations[{x, y}];
            }
        }
    }
    sum = q1 * q2 * q3 * q4;
    std::cout << sum << std::endl;

    return 0;
}
