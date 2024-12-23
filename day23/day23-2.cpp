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

    std::set<std::set<std::string>> groups;
    for (auto const &pair : graph)
    {
        std::set<std::string> conns;
        auto const node1 = pair.first;
        auto const neighbors1 = pair.second;

        for (auto const &node2 : neighbors1)
        {
            auto const &neighbors2 = graph[node2];

            for (auto const &common_neighbor : neighbors2)
            {
                if (neighbors1.count(common_neighbor))
                {
                    if (conns.empty())
                    {
                        conns.insert({node1, node2, common_neighbor});
                        continue;
                    }
                    bool qualifies = true;
                    for (auto const conn : conns)
                    {
                        if (!graph[conn].count(common_neighbor))
                            qualifies = false;
                    }
                    if (qualifies)
                    {
                        conns.insert(common_neighbor);
                    }
                }
            }
        }
        groups.insert(conns);
    }
    size_t max_length = 0;
    std::set<std::string> largest;
    for (const auto &group : groups)
    {
        if (group.size() > max_length)
        {
            max_length = group.size();
            largest = group;
        }
    }

    for (const auto &node : largest)
    {
        std::cout << node << ",";
    }
    std::cout << std::endl;

    return 0;
}
