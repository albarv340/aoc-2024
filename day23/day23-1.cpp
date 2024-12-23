#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <vector>
#include <string>

int main()
{
    std::unordered_map<std::string, std::unordered_set<std::string>> graph;

    std::ifstream file("input.txt");
    std::string line;

    if (!file.is_open())
    {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string node1, node2;
        std::getline(ss, node1, '-');
        std::getline(ss, node2);

        graph[node1].insert(node2);
        graph[node2].insert(node1);
    }

    file.close();

    std::set<std::set<std::string>> triangles;
    for (auto const &pair : graph)
    {
        auto const node1 = pair.first;
        auto const neighbors1 = pair.second;

        for (auto const &node2 : neighbors1)
        {
            auto const &neighbors2 = graph[node2];

            for (auto const &common_neighbor : neighbors2)
            {
                if (neighbors1.count(common_neighbor))
                {
                    if (node1.find('t') == 0 || node2.find('t') == 0 || common_neighbor.find('t') == 0)
                    {
                        triangles.insert({node1, node2, common_neighbor});
                    }
                }
            }
        }
    }

    std::cout << triangles.size() << std::endl;

    return 0;
}
