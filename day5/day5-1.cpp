#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

bool precedes(const std::vector<int> &vec, int value1, int value2)
{
    auto it1 = std::find(vec.begin(), vec.end(), value1);
    auto it2 = std::find(vec.begin(), vec.end(), value2);

    return it1 != vec.end() && it2 != vec.end() && it1 < it2;
}

int main()
{
    std::ifstream file("input.txt");
    if (!file.is_open())
    {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    std::string line;
    bool is_pipe_section = true;

    std::unordered_map<int, std::vector<int>> precedence;
    int sum{0};

    while (std::getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }

        if (is_pipe_section)
        {
            if (line.find('|') != std::string::npos)
            {
                std::stringstream ss(line);
                std::string temp;
                int val1, val2;
                std::getline(ss, temp, '|');
                val1 = std::stoi(temp);
                std::getline(ss, temp, '|');
                val2 = std::stoi(temp);
                precedence[val1].push_back(val2);
            }
            else
            {
                is_pipe_section = false;
            }
        }

        if (!is_pipe_section)
        {
            if (line.find(',') != std::string::npos)
            {
                std::vector<int> comma_values;
                std::stringstream ss(line);
                std::string temp;
                while (std::getline(ss, temp, ','))
                {
                    int value = std::stoi(temp);
                    comma_values.push_back(value);
                }
                bool is_correct = true;
                for (int value : comma_values)
                {
                    if (!is_correct)
                        break;
                    for (int descendant : precedence[value])
                    {
                        if (precedes(comma_values, descendant, value))
                        {
                            is_correct = false;
                            break;
                        }
                    }
                }
                if (is_correct)
                {
                    sum += comma_values[comma_values.size() / 2];
                }
            }
        }
    }
    std::cout << sum << std::endl;
    file.close();
    return 0;
}
