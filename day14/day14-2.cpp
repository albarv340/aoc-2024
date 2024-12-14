#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <map>
#include <set>
#include <thread>
#include <chrono>

struct Robot
{
    int x;
    int y;
    int vx;
    int vy;
};

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

    std::vector<Robot> robots;
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
            robots.push_back({x, y, vx, vy});
        }
    }
    for (int i = 7248; i < 100000; i += 101)
    {
        locations.clear();
        for (auto &robot : robots)
        {
            int x = robot.x;
            int y = robot.y;
            int vx = robot.vx;
            int vy = robot.vy;
            std::pair<int, int> loc{(((x + vx * i) % width) + width) % width, (((y + vy * i) % height) + height) % height};
            locations[loc] += 1;
        }
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                if (locations[{x, y}])
                {
                    std::cout << "🟩";
                }
                else
                {
                    std::cout << "⬛";
                }
            }
            std::cout << std::endl;
        }
        std::cout << "Iterations: " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return 0;
}
