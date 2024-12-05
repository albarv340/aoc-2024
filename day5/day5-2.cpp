#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

bool precedes(std::vector<int> const &vec, int value1, int value2)
{
    auto it1 = std::find(vec.begin(), vec.end(), value1);
    auto it2 = std::find(vec.begin(), vec.end(), value2);

    return it1 != vec.end() && it2 != vec.end() && it1 < it2;
}

bool is_vector_correct(std::vector<int> const &comma_values, std::unordered_map<int, std::vector<int>> &precedence)
{
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
    return is_correct;
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
                bool has_been_false = false;
                while (!is_vector_correct(comma_values, precedence))
                {
                    for (int value : comma_values)
                    {
                        for (int descendant : precedence[value])
                        {
                            auto it1 = std::find(comma_values.begin(), comma_values.end(), descendant);
                            auto it2 = std::find(comma_values.begin(), comma_values.end(), value);

                            bool precedes = it1 != comma_values.end() && it2 != comma_values.end() && it1 < it2;
                            if (precedes)
                            {
                                has_been_false = true;

                                comma_values.erase(it2);
                                comma_values.insert(it1, value);
                            }
                        }
                    }
                }
                if (has_been_false)
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
