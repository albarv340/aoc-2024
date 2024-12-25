#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <regex>
#include <deque>
#include <cmath>
#include <bitset>
#include <set>
#include <unordered_set>
#include <iomanip>
#include <sstream>

struct Operation
{
    std::string operand1;
    std::string operand2;
    std::string oper;
    std::string output;
};

long long desired_sum = 0;
std::vector<Operation> all_operations;
std::vector<std::string> relevant_registers;
std::unordered_map<std::string, std::unordered_set<std::string>> related;
std::set<std::set<std::string>> tested;

long long perform_operation(long long op1, long long op2, std::string operand)
{
    if (operand == "AND")
    {
        return op1 && op2;
    }
    if (operand == "OR")
    {
        return op1 || op2;
    }
    return op1 != op2;
}

long long get_z_sum(std::unordered_map<std::string, long long> const &registers)
{
    long long sum{0};
    for (const auto &pair : registers)
    {
        if (pair.first.find('z') == 0)
        {
            sum += pair.second * std::pow(2, std::stoll(pair.first.substr(1)));
        }
    }
    return sum;
}

std::unordered_map<std::string, long long> swap_values(std::unordered_map<std::string, long long> const &registers, std::string const &key1, std::string const &key2)
{
    std::unordered_map<std::string, long long> swapped_map = registers;

    auto it1 = swapped_map.find(key1);
    auto it2 = swapped_map.find(key2);

    if (it1 != swapped_map.end() && it2 != swapped_map.end())
    {
        std::swap(it1->second, it2->second);
    }
    return swapped_map;
}
int digit{46};
void search(std::set<std::string> swaps, std::unordered_map<std::string, long long> registers)
{
    if (tested.count(swaps))
    {
        return;
    }
    tested.insert(swaps);
    if (swaps.size() > 8)
    {
        return;
    }
    std::bitset<46> bin_sum(desired_sum);
    std::bitset<46> reg_sum(get_z_sum(registers));

    if (bin_sum == reg_sum)
    {
        for (auto const &s : swaps)
        {
            std::cout << s << ",";
        }
        std::cout << std::endl;
    }

    for (int i = 45; i >= 0; i--)
    {
        if (bin_sum[i] != reg_sum[i])
        {
            if (i >= digit)
            {
                return;
            }
            std::ostringstream oss;
            oss << std::setw(2) << std::setfill('0') << i;
            std::string key{"z" + oss.str()};
            for (const auto &r1 : relevant_registers)
                for (const auto &r2 : relevant_registers)

                {
                    if (r1 != r2)
                    {
                        auto swapped{swap_values(registers, r1, r2)};
                        if (swapped[key] == registers[key])
                        {
                            continue;
                        }
                        std::bitset<46> new_reg_sum(get_z_sum(swapped));
                        bool is_relevant = true;
                        for (int k = i; k < 46; k++)
                        {
                            if (bin_sum[k] != new_reg_sum[k])
                            {
                                is_relevant = false;
                            }
                        }
                        if (is_relevant)
                        {
                            std::set<std::string> new_swaps = swaps;
                            new_swaps.insert(r1);
                            new_swaps.insert(r2);
                            digit = i;
                            search(new_swaps, swapped);
                        }
                    }
                }
        }
    }
}

int main()
{
    std::ifstream file("input.txt");
    if (!file)
    {
        std::cerr << "Unable to open the file." << std::endl;
        return 1;
    }

    std::unordered_map<std::string, long long> registers;
    std::deque<Operation> operations;
    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty())
            break;

        std::stringstream ss(line);
        std::string key, value;

        std::getline(ss, key, ':');
        std::getline(ss, value);

        registers[key] = std::stoll(value);
    }

    std::regex operation_regex("(.+) (AND|OR|XOR) (.+) -> (.+)");
    std::smatch match;

    while (std::getline(file, line))
    {
        if (std::regex_match(line, match, operation_regex))
        {
            std::string operand1 = match[1];
            std::string oper = match[2];
            std::string operand2 = match[3];
            std::string output = match[4];
            related[output].insert(operand1);
            related[output].insert(operand2);
            related[output].insert(output);
            all_operations.push_back({operand1, operand2, oper, output});
            operations.push_back({operand1, operand2, oper, output});
        }
    }

    file.close();

    for (const auto &pair : registers)
    {
        if (pair.first.find('x') == 0)
        {
            desired_sum += pair.second * std::pow(2, std::stoll(pair.first.substr(1)));
        }
        if (pair.first.find('y') == 0)
        {
            desired_sum += pair.second * std::pow(2, std::stoll(pair.first.substr(1)));
        }
    }

    while (!operations.empty())
    {
        auto op = operations.front();
        operations.pop_front();
        if (registers.count(op.operand1) && registers.count(op.operand2))
        {
            registers[op.output] = perform_operation(registers[op.operand1], registers[op.operand2], op.oper);
        }
        else
        {
            operations.push_back(op);
        }
    }

    for (auto const &reg : registers)
    {
        if (reg.first.find('x') != 0 && reg.first.find('y') != 0)
        {
            relevant_registers.push_back(reg.first);
        }
    }

    search({}, registers);

    return 0;
}
