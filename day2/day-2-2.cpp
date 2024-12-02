#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <ranges>

bool is_safe(std::vector<int> const &vec)
{
    bool is_ascending{vec[1] > vec[0]};
    for (int i = 1; i < vec.size(); i++)
    {
        if (abs(vec[i] - vec[i - 1]) > 3 || vec[i] == vec[i - 1])
        {
            return false;
        }
        if (is_ascending && vec[i] < vec[i - 1] || !is_ascending && vec[i] > vec[i - 1])
        {
            return false;
        }
    }
    return true;
}

int main()
{
    std::ifstream input_file("input.txt");
    if (!input_file)
    {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    int safe_count{0};
    std::string line;
    while (std::getline(input_file, line))
    {
        std::istringstream line_stream(line);
        std::vector<int> numbers((std::istream_iterator<int>(line_stream)), std::istream_iterator<int>());
        for (int i = 0; i < numbers.size(); i++)
        {
            std::vector<int> combined(numbers.begin(), numbers.begin() + i);
            combined.insert(combined.end(), numbers.begin() + i + 1, numbers.end());
            if (is_safe(combined)) {
                safe_count++;
                break;
            }
        }
    }
    std::cout << safe_count << std::endl;

    input_file.close();
    return 0;
}
