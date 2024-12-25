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

long long x_sum = 0;
long long y_sum = 0;
std::unordered_map<std::string, std::vector<Operation>> related_ops;

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

int main()
{
    std::ifstream file("input.txt");
    if (!file)
    {
        std::cerr << "Unable to open the file." << std::endl;
        return 1;
    }
    // This was filled in by hand when I found obvious faults that needed swapping so I could verify they worked
    std::unordered_map<std::string, std::string> swaps{
        {"z12", "kwb"},
        {"kwb", "z12"},
        {"qkf", "z16"},
        {"z16", "qkf"},
        {"z24", "tgr"},
        {"tgr", "z24"},
        {"cph", "jqn"},
        {"jqn", "cph"},
    };

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
            if (swaps.count(output))
            {
                output = swaps[output];
            }
            related_ops[output].push_back({operand1, operand2, oper, output});
            operations.push_back({operand1, operand2, oper, output});
        }
    }

    file.close();

    for (const auto &pair : registers)
    {
        if (pair.first.find('x') == 0)
        {
            x_sum += pair.second * std::pow(2, std::stoll(pair.first.substr(1)));
        }
        if (pair.first.find('y') == 0)
        {
            y_sum += pair.second * std::pow(2, std::stoll(pair.first.substr(1)));
        }
    }

    std::ofstream file2("graph.dot");
    file2 << "graph G {" << std::endl;

    int i{0};
    while (!operations.empty())
    {
        auto op = operations.front();
        operations.pop_front();
        if (registers.count(op.operand1) && registers.count(op.operand2))
        {
            i++;
            registers[op.output] = perform_operation(registers[op.operand1], registers[op.operand2], op.oper);
            file2 << op.operand1 << " -> " << op.oper << i << ";" << std::endl;
            file2 << op.operand2 << " -> " << op.oper << i << ";" << std::endl;
            file2 << op.output << " <- " << op.oper << i << ";" << std::endl;
            for (auto const & op1 : related_ops[op.operand1])
            {
                related_ops[op.output].insert(related_ops[op.output].begin(), op1);
            }
            for (auto const & op2 : related_ops[op.operand2])
            {
                related_ops[op.output].insert(related_ops[op.output].begin(), op2);
            }
        }
        else
        {
            operations.push_back(op);
        }
    }
    file2 << "}" << std::endl;
    file2.close();

    long long decimal_value = 0;
    for (const auto &pair : registers)
    {
        if (pair.first.find('z') == 0)
        {
            decimal_value += pair.second * std::pow(2, std::stoll(pair.first.substr(1)));
        }
    }
    std::bitset<46> bit_x_sum(x_sum);
    std::bitset<46> bit_y_sum(y_sum);
    std::bitset<46> bin_sum(x_sum + y_sum);
    std::bitset<46> reg_sum(decimal_value);
    // std::cout << "Y:        " << bit_x_sum << std::endl;
    // std::cout << "X:        " << bit_y_sum << std::endl;
    // std::cout << "Expected: " << bin_sum << std::endl;
    // std::cout << "Actual:   " << reg_sum << std::endl;
    // std::ofstream file3("graph2.dot");
    // file3 << "graph G {" << std::endl;
    // i = 0;
    // for (auto const & rel : related_ops["z29"]) // Manual checks and changing this value by hand
    // {
    //     std::cout << rel.operand1 << " " << rel.oper << " " << rel.operand2 << " " << rel.output << std::endl;
    //     i++;
    //     file3 << rel.operand1 << " -- " << rel.oper << i << ";" << std::endl;
    //     file3 << rel.operand2 << " -- " << rel.oper << i << ";" << std::endl;
    //     file3 << rel.output << " -- " << rel.oper << i << ";" << std::endl;
    // }
    // file3 << "}" << std::endl;
    // file3.close();
    // Addition needs an XOR from the same bit e.g. X16 XOR Y16 -> abc (1 + 1 = 10 (0 and carry bit 1), and 0 + 1 = 1 and 0 + 0 = 0), and an 
    // AND from previous bit (carry bit) e.g. X15 AND Y15 -> bcd (will only have a carry bit if previous added up to 10 or 11, this means 
    // previous carry bit also needs to be taken into consideration) and then abc XOR bcd -> Z16
    // The input is a lot more jumbled but with enough patience I could spot "obvious" faults in the graphs and input and find four relevant swaps
    // C: 1    1   1          11         111      11  1
    // Y: 0110101100100010110001101000101011111011001101 +
    // X: 0101001000101101000000100100010000100000011001
    //    1011110101001111110010001100111100011011100110
    std::set<std::string> res;
    for (auto const & swap : swaps)
    {
        res.insert(swap.first);
    }
    for (auto const & r : res)
    {
        std::cout << r << ",";
    }
    std::cout << std::endl;
    return 0;
}
