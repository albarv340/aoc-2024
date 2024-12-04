#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <ranges>
#include <iterator>
#include <vector>

bool check_MAS(char const &a, char const &b, char const &c)
{
    return a == 'M' && b == 'A' && c == 'S';
}

int main()
{
    std::ifstream file("input.txt");

    if (!file.is_open())
    {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    std::string input((std::istreambuf_iterator<char>(file)),
                      std::istreambuf_iterator<char>());

    std::istringstream stream(input);
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(stream, line))
    {
        lines.push_back(line);
    }

    file.close();
    input.erase(std::remove(input.begin(), input.end(), '\n'), input.end());
    int height = lines.size();
    int width = lines[0].size();

    int amount{0};

    std::vector<std::pair<int, int>> directions = {
        {-1, -1}, {1, 1}, {-1, 1}, {1, -1} // Diagonals
    };

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int local_mas_amount{0};
            for (auto &dir : directions)
            {
                int dx = dir.first;
                int dy = dir.second;
                if (x + dx < width && x + dx >= 0 && y + dy < height && y + dy >= 0 && x - dx < width && x - dx >= 0 && y - dy < height && y - dy >= 0)
                {
                    if (check_MAS(input[(x - dx) + (y - dy) * width], input[(x) + (y)*width], input[x + dx + (y + dy) * width]))
                    {
                        local_mas_amount++;
                    }
                }
            }
            if (local_mas_amount >= 2)
            {
                amount++;
            }
        }
    }

    std::cout << amount << std::endl;

    return 0;
}
