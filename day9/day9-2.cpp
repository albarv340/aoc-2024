#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <algorithm>

bool can_fit_n(std::vector<long long> const &vec, long long start_index, long long n)
{
    if (start_index + n > vec.size())
    {
        return false;
    }

    for (long long i = 0; i < n; ++i)
    {
        if (vec[start_index + i] != -1)
        {
            return false;
        }
    }
    return true;
}

int main()
{
    std::ifstream file("input.txt");

    if (!file)
    {
        std::cerr << "Unable to open the file.\n";
        return 1;
    }

    std::string line;
    std::getline(file, line);

    file.close();

    std::vector<long long> file_representation;
    std::vector<long long> ids;

    long long num;
    long long index = 0;

    for (long long i = 0; i < line.length(); i++)
    {
        long long value = line[i] - '0';
        if (i % 2 == 0)
        {
            file_representation.insert(file_representation.end(), value, index);
            ids.push_back(index);
            index++;
        }
        else
        {
            file_representation.insert(file_representation.end(), value, -1);
        }
    }


    for (auto it = ids.rbegin(); it != ids.rend(); ++it)
    {
        auto first = std::find(file_representation.begin(), file_representation.end(), *it);
        auto last = std::find(file_representation.rbegin(), file_representation.rend(), *it);

        long long size{std::distance(first, last.base())};
        for (long long i = 0; i < file_representation.size(); i++)
        {
            if (file_representation[i] != -1)
            {
                continue;
            }
            if (can_fit_n(file_representation, i, size) && std::distance(first, file_representation.begin() + i) < 0)
            {
                std::vector<int> moved_elements(first, last.base());
                std::fill(first, last.base(), -1);
                file_representation.erase(file_representation.begin() + i, file_representation.begin() + i + size);
                file_representation.insert(file_representation.begin() + i, moved_elements.begin(), moved_elements.end());
                break;
            }
        }
    }

    long long checksum{0};

    for (long long i = 0; i < file_representation.size(); i++)
    {
        if (file_representation[i] != -1)
        {
            checksum += file_representation[i] * i;
        }
    }
    std::cout << checksum << std::endl;

    return 0;
}
