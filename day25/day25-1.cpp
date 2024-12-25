#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

int main()
{
    std::ifstream file("input.txt");
    if (!file)
    {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    std::vector<std::vector<int>> key;
    std::vector<std::vector<int>> lock;

    std::string line;
    std::vector<std::string> group(7);

    while (true)
    {
        bool is_group_full = true;
        for (int i = 0; i < 7; ++i)
        {
            if (!std::getline(file, group[i]))
            {
                is_group_full = false;
                break;
            }
        }

        if (!is_group_full)
            break;
        std::string newline;
        std::getline(file, newline);

        bool is_lock = true;
        for (char c : group[0])
        {
            if (c != '#')
            {
                is_lock = false;
                break;
            }
        }

        std::vector<int> hashtag_counts(5, 0);
        int start_i = is_lock ? 1 : 0;
        int end_i = is_lock ? 7 : 6;
        for (int i = start_i; i < end_i; ++i)
        {
            for (size_t j = 0; j < 5; ++j)
            {
                if (group[i][j] == '#')
                {
                    hashtag_counts[j]++;
                }
            }
        }

        if (is_lock)
        {
            lock.push_back(hashtag_counts);
        }
        else
        {
            key.push_back(hashtag_counts);
        }
    }
    file.close();

    long long sum{0};
    for (const auto &k_row : key)
    {
        for (const auto &l_row : lock)
        {
            bool fits = true;
            for (int i = 0; i < 5; i++)
            {
                if (k_row[i] + l_row[i] > 5)
                {
                    fits = false;
                }
            }
            if (fits)
            {
                sum++;
            }
        }
    }

    std::cout << sum << std::endl;
}
