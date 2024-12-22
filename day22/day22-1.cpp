#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <cmath>
#include <climits>


int main()
{
    std::ifstream file("input.txt");
    std::string line;
    long long number;

    if (!file)
    {
        std::cerr << "Error opening the file!" << std::endl;
        return 1;
    }
    long long tot{0};
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        ss >> number;
        for (int i = 0; i < 2000; i++)
        {
            long long op1 = number << 6;
            number ^= op1;
            number %= 16777216;
            long long op2 = number >> 5;
            number ^= op2;
            number %= 16777216;
            long long op3 = number << 11;
            number ^= op3;
            number %= 16777216;

        }
        tot += number;
    }

    std::cout << tot << std::endl;

    file.close();

    return 0;
}
