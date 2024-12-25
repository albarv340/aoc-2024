#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <regex>
#include <deque>
#include <cmath>

struct Operation
{
    std::string operand1;
    std::string operand2;
    std::string oper;
    std::string output;
};

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
            operations.push_back({operand1, operand2, oper, output});
        }
    }

    file.close();

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


    long long decimal_value = 0;
    for (const auto &pair : registers)
    {
        if (pair.first.find('z') == 0)
        {
            decimal_value += pair.second * std::pow(2, std::stoll(pair.first.substr(1)));
        }
    }

    std::cout << decimal_value << std::endl;

    return 0;
}
