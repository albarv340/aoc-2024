#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

int main()
{
    std::ifstream file("input.txt");
    std::vector<long long> numbers;
    long long num;

    if (file.is_open())
    {
        while (file >> num)
        {
            numbers.push_back(num);
        }
        file.close();
    }
    else
    {
        std::cerr << "Unable to open the file!" << std::endl;
    }
    for (int i = 0; i < 25; i++)
    {
        for (long long j = 0; j < numbers.size(); j++)
        {
            if (numbers[j] == 0)
            {
                numbers[j] = 1;
                continue;
            }
            int num_digits = (int)floor(log10(numbers[j])) + 1;
            if (num_digits % 2 == 0)
            {
                std::string numStr = std::to_string(numbers[j]);

                int n = num_digits / 2;
                long long first_half = std::stoll(numStr.substr(0, n));
                long long second_half = std::stoll(numStr.substr(n));
                numbers.erase(numbers.begin() + j);
                numbers.insert(numbers.begin() + j, second_half);
                numbers.insert(numbers.begin() + j, first_half);
                j++;
                continue;
            }
            numbers[j] *= 2024;
        }
    }
    std::cout << numbers.size() << std::endl;

    return 0;
}
