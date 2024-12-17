#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <cmath>

long long A, B, C;

long long get_combo(long long num)
{
    if (num <= 3)
        return num;
    if (num == 4)
        return A;
    if (num == 5)
        return B;
    if (num == 6)
        return C;
    return -1;
}

int main()
{
    std::ifstream input("input.txt");
    if (!input)
    {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    std::string line;
    std::vector<int> program;

    std::regex register_regex("Register ([A-C]): (\\d+)");
    std::regex program_regex("Program: ([0-9,]+)");
    std::smatch match;

    while (std::getline(input, line))
    {
        if (std::regex_search(line, match, register_regex))
        {
            std::string register_name = match[1].str();
            long long register_value = std::stoll(match[2].str());

            if (register_name == "A")
            {
                A = register_value;
            }
            else if (register_name == "B")
            {
                B = register_value;
            }
            else if (register_name == "C")
            {
                C = register_value;
            }
        }
        else if (std::regex_search(line, match, program_regex))
        {
            std::string programStr = match[1].str();
            std::regex num_regex("\\d+");
            auto nums_begin = std::sregex_iterator(programStr.begin(), programStr.end(), num_regex);
            auto nums_end = std::sregex_iterator();

            for (auto it = nums_begin; it != nums_end; ++it)
            {
                program.push_back(std::stoi(it->str()));
            }
        }
    }

    input.close();
    std::vector<int> out;
    int ip{0};
    while (ip < program.size())
    {
        int op = program[ip];
        int in = program[ip + 1];

        switch (op)
        {
        case 0:
            A = A / pow(2, get_combo(in));
            break;
        case 1:
            B = B ^ in;
            break;
        case 2:
            B = get_combo(in) % 8;
            break;
        case 3:
            if (A != 0)
            {
                ip = in;
                continue;
            }
            break;
        case 4:
            B = B ^ C;
            break;
        case 5:
            out.push_back(get_combo(in) % 8);
            break;
        case 6:
            B = A / pow(2, get_combo(in));
            break;
        case 7:
            C = A / pow(2, get_combo(in));
            break;
        default:
            break;
        }

        ip += 2;
    }

    for (int num : out)
    {
        std::cout << num << ",";
    }
    std::cout << std::endl;

    return 0;
}
