#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <set>

bool evaluate(long long target, std::vector<long long> const & values)
{
    std::stringstream ss;
    ss << values[0] << values[1];
    if (values.size() == 2) {
        return ((values[0] + values[1] == target) || values[0] * values[1] == target || std::stoll(ss.str()) == target);
    }
    std::vector<long long> concatenated_copy = {std::stoll(ss.str())};
    std::vector<long long> plus_copy = {values[0] + values[1]};
    std::vector<long long> mult_copy = {values[0] * values[1]};
    concatenated_copy.insert(concatenated_copy.end(), values.begin() + 2, values.end());
    plus_copy.insert(plus_copy.end(), values.begin() + 2, values.end());
    mult_copy.insert(mult_copy.end(), values.begin() + 2, values.end());


    return evaluate(target, concatenated_copy) || evaluate(target, plus_copy) || evaluate(target, mult_copy);
}

int main()
{
    std::ifstream file("input.txt");

    if (!file.is_open())
    {
        std::cerr << "Failed to open the file!" << std::endl;
        return 1;
    }

    std::string line;
    long long sum{0};
    while (std::getline(file, line))
    {
        size_t colon_pos = line.find(':');
        long long target = std::stoll(line.substr(0, colon_pos));

        std::string values = line.substr(colon_pos + 1);

        std::istringstream value_stream(values);

        std::vector<long long> value_vec((std::istream_iterator<long long>(value_stream)),
                                   std::istream_iterator<long long>());

        if (evaluate(target, value_vec)) {
            sum += target;
        }
    }

    std::cout << sum << std::endl;

    file.close();

    return 0;
}
